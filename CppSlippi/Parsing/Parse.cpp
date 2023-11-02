#include "Parse.h"

#include <cstdio>
#include <cstring>
#include <concepts>
#include <format>
#include <optional>
#include <unordered_map>
#include <variant>

#include <nlohmann/json.hpp>

#include "CppSlippi/Model.h"
#include "CppSlippi/Parsing/BlockReader.h"
#include "CppSlippi/Parsing/FrameEventProcessor.h"
#include "CppSlippi/Parsing/ParseException.h"
#include "CppSlippi/Parsing/ShiftJis.h"
#include "CppSlippi/Parsing/Utils.h"
#include "CppSlippi/Reading/FixedBufferReader.h"


using cpp_slippi::reading::Reader;


namespace cpp_slippi::parsing {
namespace {

// Slippi events that can appear in a game's raw data.
enum class EventType : uint8_t {
	MESSAGE_SPLITTER = 0x10,
	EVENT_PAYLOADS = 0x35,
	GAME_START = 0x36,
	FRAME_PRE = 0x37,
	FRAME_POST = 0x38,
	GAME_END = 0x39,
	FRAME_START = 0x3A,
	FRAME_ITEM = 0x3B,
	FRAME_END = 0x3C,
	GECKO_CODES = 0x3D,
};


struct MessageSplitter {
	struct Payload {
		uint8_t bytes[0x200];
	};

	Payload payload;
	uint16_t actualSize;
	EventType internalCommand;
	bool lastMessage;
};


std::pair<uint32_t, std::unordered_map<EventType, uint16_t>> parseEventPayloads(Reader& reader) {
	std::pair<uint32_t, std::unordered_map<EventType, uint16_t>> result;

	BlockReader block(reader);
	block.expect(0x0, EventType::EVENT_PAYLOADS);
	auto eventSize = block.read<uint8_t>(0x1);
	result.first = eventSize + 1;

	// Each event takes 3 bytes, plus the size bytes.
	if (eventSize % 3 != 1) {
		throw ParseException(std::format("Invalid event payloads size, must be 1 mod 3. Was: {}", eventSize));
	}

	uint8_t commandCount = eventSize/3;
	block.readFor(0x2, commandCount, 0x3, [&result](BlockReader& reader, size_t i) {
		auto type = reader.read<EventType>(0x0);
		// Size does not include the command byte, so we add that here.
		result.second[type] = reader.read<uint16_t>(0x1) + 1;
	});
	return result;
}

nlohmann::json readMetadata(Reader& reader) {
	reader.expectBytes("U\x08metadata");
	auto rawMetadata = nlohmann::json(nlohmann::json::from_ubjson(reader.begin(), reader.end(), /*strict=*/false));
	reader.expectBytes("}");
	return rawMetadata;
}

// Skip to the end of the stream to read the metadata, if possible.
std::optional<nlohmann::json> skipReadMetadata(
		Reader& reader, std::optional<uint32_t> length, const std::unordered_map<EventType, uint16_t>& payloadSizes) {
	if (!length || !reader.seekable()) {
		return std::nullopt;
	}
	size_t savedPosition = reader.position();
	reader.seek(*length);
	nlohmann::json rawMetadata = readMetadata(reader);
	reader.seek(savedPosition, SEEK_SET);

	return rawMetadata;
}

// Determine the maximum possible number of frames in the replay. This cannot
// be exact because we don't know the number of items or split messages (Gecko
// Codes) that are in the replay.
uint32_t estimateNumFramesFromPayloadSizes(
		const Game::Start& start,
		uint32_t length,
		const std::unordered_map<EventType, uint16_t>& payloadSizes) {
	// Count the number of characters in the game. Ice Climbers have a follower
	// so they count for two.
	uint32_t numCharacters = 0;
	for (const auto& maybePlayer : start.players) {
		if (maybePlayer) {
			numCharacters += maybePlayer->character == CssCharacter::ICE_CLIMBERS ? 2 : 1;
		}
	}

	auto maybeStartSize = payloadSizes.find(EventType::FRAME_START);
	auto maybePreSize = payloadSizes.find(EventType::FRAME_PRE);
	auto maybePostSize = payloadSizes.find(EventType::FRAME_POST);
	auto maybeEndSize = payloadSizes.find(EventType::FRAME_END);

	// Add up the minimum size for a frame.
	uint32_t frameSize = 0;
	if (maybeStartSize != payloadSizes.end()) {
		frameSize += maybeStartSize->second;
	}
	if (maybePreSize != payloadSizes.end()) {
		frameSize += numCharacters * maybePreSize->second;
	}
	if (maybePostSize != payloadSizes.end()) {
		frameSize += numCharacters * maybePostSize->second;
	}
	if (maybeEndSize != payloadSizes.end()) {
		frameSize += maybeEndSize->second;
	}

	auto maybeGameEnd = payloadSizes.find(EventType::GAME_END);
	if (maybeGameEnd != payloadSizes.end()) {
		length -= maybeGameEnd->second;
	}

	return length / frameSize + 1;
}

template<typename T>
std::optional<T> tryGet(const nlohmann::json& json, const std::string& key) {
	if (!json.contains(key)) {
		return std::nullopt;
	}
	return json.at(key).get<T>();
}

Metadata  parseMetadata(const nlohmann::json& rawMetadata) {
	Metadata metadata = {
		.startAt = tryGet<std::string>(rawMetadata, "startAt"),
		.lastFrame = tryGet<int32_t>(rawMetadata, "lastFrame"),
		.playedOn = tryGet<std::string>(rawMetadata, "playedOn"),
		.consoleNick = tryGet<std::string>(rawMetadata, "consoleNick"),
	};

	if (rawMetadata.contains("players")) {
		const nlohmann::json& playersJson = rawMetadata.at("players");
		auto& players = metadata.players.emplace();
		for (const auto& [key, rawPlayer] : playersJson.items()) {
			size_t port = std::stoi(key);
			Metadata::Player& player = players[port].emplace();

			if (rawPlayer.contains("characters")) {
				for (const auto& [charStr, value] : rawPlayer.at("characters").items()) {
					auto character = static_cast<InGameCharacter>(std::stoi(charStr));
					player.characters[character] = value.get<uint32_t>();
				}
			}

			if (rawPlayer.contains("names")) {
				for (const auto& [name, value] : rawPlayer.at("names").items()) {
					player.names[name] = value.get<std::string>();
				}
			}
		}
	}
	return metadata;
}


class GeckoCodesParser {
public:
	GeckoCodesParser(Options options, Handlers& handlers) :
		options_(options), handlers_(handlers) {}

	void parseChunk(MessageSplitter& messageSplitter) {
		if (!(options_.interestedEvents & Options::InterestedEvents::GECKO_CODES)) {
			return;
		}

		if (messageSplitter.internalCommand != EventType::GECKO_CODES) {
			throw ParseException("Out of order split messages are not supported.");
		}

		size_t size = geckoCodes_.bytes.size();
		geckoCodes_.bytes.resize(size + messageSplitter.actualSize);
		memcpy(geckoCodes_.bytes.data() + size, messageSplitter.payload.bytes, messageSplitter.actualSize);

		if (messageSplitter.lastMessage) {
			handlers_.geckoCodes(geckoCodes_);
			geckoCodes_.bytes.resize(0);
		}
	}

private:
	Options options_;
	Handlers& handlers_;
	GeckoCodes geckoCodes_;
};


class EventParser {
public:
	EventParser(
		Options options,
		Handlers& handlers,
		const std::unordered_map<EventType, uint16_t>& payloadSizes,
		std::optional<uint32_t> numFramesFromMetadata,
		Reader& stream) :
			// Initialization of frameEventProcessor is deferred until we read
			// GAME_START. This requires that GAME_START is the first event,
			// which seems to be fine.
			frameEventProcessor_(nullptr),
			options_(options),
			shiftJis_(options.fullwidthToHalfwidth),
			handlers_(handlers),
			payloadSizes_(payloadSizes),
			numFramesFromMetadata_(numFramesFromMetadata),
			stream_(stream) {}

	void parseEvents(std::optional<uint32_t> length) {
		uint32_t bytesRead = 0;
		EventType lastEvent = EventType::EVENT_PAYLOADS;

		// We must check for GAME_END as well because we don't know the length
		// of streaming files.
		while ((!length || bytesRead <= *length) && lastEvent != EventType::GAME_END) {
			// Skip to the end if the only remaining interested event is
			// GAME_END and we have a frameEventProcessor (requires parsing
			// GAME_START).
			if (length && options_.interestedEvents == Options::InterestedEvents::GAME_END && frameEventProcessor_) {
				uint16_t gameEndSize = payloadSizes_.at(EventType::GAME_END);
				if (*length - bytesRead < gameEndSize) {
					throw ParseException(std::format("Want to skip to GAME_END event, but only {} bytes available ({} bytes required for GAME_END).",
						*length - bytesRead, gameEndSize));
				}
				uint32_t skip = *length - bytesRead - gameEndSize;
				stream_.seek(skip);
				bytesRead += skip;
			}

			size_t payloadStart = stream_.position();

			auto [eventSize, eventType] = parseEvent(length, bytesRead);
			bytesRead += eventSize;
			lastEvent = eventType;

			// It's possible that the event didn't actually read to the end, so
			// position the stream for the next event.
			stream_.seek(payloadStart + eventSize - stream_.position());
		}
	}

private:
	std::pair<uint16_t, EventType> parseEvent(std::optional<uint32_t> length, uint32_t bytesRead) {
		BlockReader reader(stream_);
		EventType eventType = reader.read<EventType>(0x0);

		auto it = payloadSizes_.find(eventType);
		if (it == payloadSizes_.end()) {
			throw ParseException(std::format("Found unrecognized event type {} at position {}. Unknown size, aborting.",
				static_cast<uint8_t>(eventType),
				stream_.position()));
		}
		uint16_t size = it->second;
		switch (eventType) {
			case EventType::MESSAGE_SPLITTER: {
				parseMessageSplitter(reader, size);
				break;
			} case EventType::GAME_START: {
				// Always parse Game::Start because it provides some useful
				// information.
				Game::Start start = parseGameStart(reader, size);

				// For Rollback::ALL we can get an upperbound on the number of
				// frames from the payload sizes and remaining bytes. This is
				// better than the frames from metadata because it accounts for
				// rollback.
				std::optional<uint32_t> hintNumFrames = numFramesFromMetadata_;
				if (options_.rollback == Options::Rollback::ALL && length) {
					hintNumFrames = estimateNumFramesFromPayloadSizes(start, *length - bytesRead, payloadSizes_);
				}

				if (hintNumFrames) {
					handlers_.hintNumFrames(*hintNumFrames);
				}

				// There is no rollback before 3.6.0, so use the simplest frame
				// processor.
				frameEventProcessor_ = start.slippi.version < Slippi::Version{3, 6, 0, 0} ?
					FrameEventProcessor::create(Options::Rollback::ALL, handlers_, hintNumFrames) :
					FrameEventProcessor::create(options_.rollback, handlers_, hintNumFrames);

				if (options_.interestedEvents & Options::InterestedEvents::GAME_START) {
					// There is only one GAME_START, so we are no longer
					// interested in this event. Clearing this flag allows us to
					// skip to GAME_END if it is the only interested event left.
					options_.interestedEvents &= ~Options::InterestedEvents::GAME_START;

					handlers_.gameStart(start);
				}
				break;
			} case EventType::GAME_END: {
				frameEventProcessor_->finish();
				if (options_.interestedEvents & Options::InterestedEvents::GAME_END) {
					Game::End end = parseGameEnd(reader, size);
					handlers_.gameEnd(end);
				}
				break;
			} case EventType::FRAME_START: {
				if (options_.interestedEvents & Options::InterestedEvents::FRAME_START) {
					Frame::Start start = parseFrameStart(reader, size);
					frameEventProcessor_->processStart(start);
				}
				break;
			} case EventType::FRAME_PRE: {
				if (options_.interestedEvents & Options::InterestedEvents::FRAME_PRE) {
					Pre pre = parseFramePre(reader, size);
					frameEventProcessor_->processPre(pre);
				}
				break;
			} case EventType::FRAME_POST: {
				if (options_.interestedEvents & Options::InterestedEvents::FRAME_POST) {
					Post post = parseFramePost(reader, size);
					frameEventProcessor_->processPost(post);
				}
				break;
			} case EventType::FRAME_ITEM: {
				if (options_.interestedEvents & Options::InterestedEvents::FRAME_ITEM) {
					Item item = parseItem(reader, size);
					frameEventProcessor_->processItem(item);
				}
				break;
			} case EventType::FRAME_END: {
				if (options_.interestedEvents & Options::InterestedEvents::FRAME_END) {
					Frame::End end = parseFrameEnd(reader, size);
					frameEventProcessor_->processEnd(end);
				}
				break;
			} case EventType::GECKO_CODES: {
				throw ParseException("Gecko Codes are only supported through Message Splitter.");
				break;
			} default: {
				// Unknown event type, just skip it.
				break;
			}
		}
		return {size, eventType};
	}

	void parseMessageSplitter(BlockReader& reader, size_t size) {
		MessageSplitter splitter = {
			.payload = reader.read<MessageSplitter::Payload>(0x1),
			.actualSize = reader.read<uint16_t>(0x201),
			.internalCommand = reader.read<EventType>(0x203),
			.lastMessage = reader.read<bool>(0x204),
		};

		std::visit([&](auto& parser) {
			using T = decltype(parser);
			if constexpr (std::same_as<T, std::monostate&>) {
				// Construct a new parser of the appropriate type and pass the
				// message splitter to it.
				switch (splitter.internalCommand) {
					case EventType::GECKO_CODES: {
						splitMessageParser_.emplace<GeckoCodesParser>(options_, handlers_)
							.parseChunk(splitter);
						break;
					} default: {
						throw ParseException(std::format("Message splitting not supported for event type {}.",
							static_cast<uint8_t>(splitter.internalCommand)));
					}
				}
			} else {
				// The parser already exists, so just pass the chunk to it. We
				// do not support changing the internal command in the middle of
				// a split message stream.
				parser.parseChunk(splitter);
			}
		}, splitMessageParser_);

		// If that was the last message then reset the split message parser.
		if (splitter.lastMessage) {
			splitMessageParser_.emplace<std::monostate>();
		}
	}

	Game::Start parseGameStart(BlockReader& reader, size_t size) {
		Game::Start start;

		start.slippi.version = reader.read<Slippi::Version>(0x1);

		// Game Info Block.
		reader.readSubBlock(0x5, 312, [&start](BlockReader& reader) {
			start.bitfield = reader.read<Game::Bitfield>(0x0);
			start.isRainingBombs = reader.read<bool>(0x6);
			start.isTeams = reader.read<bool>(0x8);
			start.itemSpawnFrequency = reader.read<ItemSpawnFrequency>(0xB);
			start.selfDestructScore = reader.read<int8_t>(0xC);
			start.stage = reader.read<Stage>(0xE);
			start.timer = reader.read<uint32_t>(0x10);
			start.itemSpawns = reader.read<ItemSpawns, 5>(0x23);
			start.damageRatio = reader.read<float>(0x30);

			// There are actually 6 blocks for player data, but only the first four
			// are used.
			reader.readFor(0x60, 4, 0x24, [&start](BlockReader& reader, size_t i) {
				auto character = reader.read<CssCharacter>(0x0);
				auto type = reader.read<Player::Type>(0x1);
				if (type == Player::Type::EMPTY) {
					start.players[i] = std::nullopt;
					return;
				}

				Player& player = start.players[i].emplace();
				player.character = character;
				player.type = type;
				player.stocks = reader.read<uint8_t>(0x2);
				player.costume = reader.read<Costume>(0x3);
				auto shade = reader.read<Team::Shade>(0x7);
				player.handicap = reader.read<uint8_t>(0x8);
				auto color = reader.read<Team::Color>(0x9);
				player.bitfield = reader.read<Player::Bitfield>(0xC);
				player.cpuLevel = reader.read<uint8_t>(0xF);
				player.damageStart = reader.read<uint16_t>(0x10);
				player.damageSpawn = reader.read<uint16_t>(0x12);
				player.offenseRatio = reader.read<float>(0x18);
				player.defenseRatio = reader.read<float>(0x1C);
				player.modelScale = reader.read<float>(0x20);

				if (start.isTeams) {
					player.team = {
						.color = color,
						.shade = shade,
					};
				}
			});
		});
		start.randomSeed = reader.read<uint32_t>(0x13D);

		start.timerBehavior = static_cast<TimerBehavior>(start.bitfield & Game::Bitfield::TIMER_BEHAVIOR_MASK);
		start.gameMode = static_cast<GameMode>(static_cast<uint8_t>(start.bitfield & Game::Bitfield::GAME_MODE_MASK) >> 5);

		// version < 1.0.0
		if (size <= 0x141) {
			return start;
		}

		reader.readFor(0x141, 4, 0x8, [&start](BlockReader& reader, size_t i) {
			if (!start.players[i]) {
				return;
			}

			start.players[i]->ucf = {
				.dashBack = reader.read<Ucf::DashBack>(0x0),
				.shieldDrop = reader.read<Ucf::ShieldDrop>(0x4),
			};
		});

		// version < 1.3.0
		if (size <= 0x161) {
			return start;
		}

		reader.readFor(0x161, 4, 0x10, [this, &start](BlockReader& reader, size_t i) {
			if (!start.players[i]) {
				return;
			}

			struct NameTag {
				char shiftJis[16];
			};
			auto nameTag = reader.read<NameTag>(0x0);

			start.players[i]->tag = shiftJis_.toUtf8(std::string_view(nameTag.shiftJis, strnlen(nameTag.shiftJis, 16)));
		});

		// version < 1.5.0
		if (size <= 0x1A1) {
			return start;
		}

		start.isPal = reader.read<bool>(0x1A1);

		// version < 2.0.0
		if (size <= 0x1A2) {
			return start;
		}

		start.isFrozenPS = reader.read<bool>(0x1A2);

		// version < 3.7.0
		if (size <= 0x1A3) {
			return start;
		}

		start.scene = {
			.minor = reader.read<uint8_t>(0x1A3),
			.major = reader.read<uint8_t>(0x1A4),
		};

		// version < 3.9.0
		if (size <= 0x1A5) {
			return start;
		}

		reader.readFor(0x1A5, 4, 0x1F, [this, &start](BlockReader& reader, size_t i) {
			if (!start.players[i]) {
				return;
			}

			struct DisplayName {
				char shiftJis[0x1E];
			};
			auto name = reader.read<DisplayName>(0x0);

			getOrCreate(start.players[i]->netplay).name =
				shiftJis_.toUtf8(std::string_view(name.shiftJis, strnlen(name.shiftJis, 0x1E)));
		});
		reader.readFor(0x221, 4, 0xA, [this, &start](BlockReader& reader, size_t i) {
			if (!start.players[i]) {
				return;
			}

			struct ConnectCode {
				char shiftJis[0x9];
			};
			auto connectCode = reader.read<ConnectCode>(0x0);

			getOrCreate(start.players[i]->netplay).code =
				shiftJis_.toUtf8(std::string_view(connectCode.shiftJis, strnlen(connectCode.shiftJis, 0x9)));
		});

		// version < 3.11.0
		if (size <= 0x249) {
			return start;
		}

		reader.readFor(0x249, 4, 0x1D, [&start](BlockReader& reader, size_t i) {
			if (!start.players[i]) {
				return;
			}

			struct SlippiUuid {
				char uuid[0x1C];
			};
			auto slippiUuid = reader.read<SlippiUuid>(0x0);

			getOrCreate(start.players[i]->netplay).slippiUid = std::string(slippiUuid.uuid, strnlen(slippiUuid.uuid, 0x1C));
		});

		// version < 3.12.0
		if (size <= 0x2BD) {
			return start;
		}

		start.language = reader.read<Language>(0x2BD);

		if (size <= 0x2BE) {
			return start;
		}

		struct MatchId {
			char id[50];
		};
		auto matchId = reader.read<MatchId>(0x2BE);

		start.matchId = std::string(matchId.id, strnlen(matchId.id, 50));
		start.matchNumber = reader.read<uint32_t>(0x2F1);
		start.tiebreakerNumber = reader.read<uint32_t>(0x2F5);
		return start;
	}

	Game::End parseGameEnd(BlockReader& reader, size_t size) {
		Game::End end;
		end.method = reader.read<Game::End::Method>(0x1);

		// version < 2.0.0
		if (size <= 0x2) {
			return end;
		}
		end.lrasInitiator = reader.read<Port>(0x2);

		// version < 3.13.0
		if (size <= 0x3) {
			return end;
		}
		end.playerPlacements = std::array<int8_t, 4>{};
		reader.readFor(0x3, 4, 1, [&end](BlockReader& reader, size_t i) {
			end.playerPlacements->at(i) = reader.read<Port>(0x0);
		});
		return end;
	}

	Frame::Start parseFrameStart(BlockReader& reader, size_t size) {
		Frame::Start start;
		start.frameNumber = reader.read<int32_t>(0x1);
		start.randomSeed = reader.read<uint32_t>(0x5);

		// version < 3.10.0
		if (size <= 0x9) {
			return start;
		}
		start.sceneFrameCounter = reader.read<uint32_t>(0x9);
		return start;
	}

	Pre parseFramePre(BlockReader& reader, size_t size) {
		Pre pre;
		pre.frameNumber = reader.read<int32_t>(0x1);
		pre.port = reader.read<Port>(0x5);
		pre.isFollower = reader.read<bool>(0x6);
		pre.randomSeed = reader.read<uint32_t>(0x7);
		pre.state = reader.read<ActionState>(0xB);
		pre.position = {
			.x = reader.read<float>(0xD),
			.y = reader.read<float>(0x11),
		};
		pre.direction = reader.read<float>(0x15);
		pre.joystick = {
			.x = reader.read<float>(0x19),
			.y = reader.read<float>(0x1D),
		};
		pre.cstick = {
			.x = reader.read<float>(0x21),
			.y = reader.read<float>(0x25),
		};
		pre.triggers.logical = reader.read<float>(0x29);
		pre.buttons = {
			.logical = reader.read<Buttons::Logical>(0x2D),
			.physical = reader.read<Buttons::Physical>(0x31),
		};
		pre.triggers.physical = {
			.left = reader.read<float>(0x33),
			.right = reader.read<float>(0x37),
		};

		// version < 1.2.0
		if (size <= 0x3B) {
			return pre;
		}
		pre.rawAnalogX = reader.read<uint8_t>(0x3B);

		// version < 1.4.0
		if (size <= 0x3C) {
			return pre;
		}
		pre.damage = reader.read<float>(0x3C);
		return pre;
	}

	Post parseFramePost(BlockReader& reader, size_t size) {
		Post post;
		post.frameNumber = reader.read<int32_t>(0x1);
		post.port = reader.read<Port>(0x5);
		post.isFollower = reader.read<bool>(0x6);
		post.character = reader.read<InGameCharacter>(0x7);
		post.state = reader.read<ActionState>(0x8);
		post.position = {
			.x = reader.read<float>(0xA),
			.y = reader.read<float>(0xE),
		};
		post.direction = reader.read<float>(0x12);
		post.damage = reader.read<float>(0x16);
		post.shield = reader.read<float>(0x1A);
		post.lastAttackLanded = reader.read<Attack>(0x1E);
		post.comboCount = reader.read<uint8_t>(0x1F);
		post.lastHitBy = reader.read<Attack>(0x20);
		post.stocks = reader.read<uint8_t>(0x21);

		// version < 1.2.0
		if (size <= 0x22) {
			return post;
		}
		post.stateAge = reader.read<float>(0x22);

		if (size <= 0x26) {
			return post;
		}
		post.stateFlags = reader.read<StateFlags, 5>(0x26);
		post.hitStunRemaining = reader.read<float>(0x2B);
		post.airborne = reader.read<bool>(0x2F);
		post.lastGroundId = reader.read<uint16_t>(0x30);
		post.jumpsRemaining = reader.read<uint8_t>(0x32);
		post.lCancel = reader.read<LCancel>(0x33);

		// version < 2.0.0
		if (size <= 0x34) {
			return post;
		}

		post.hurtboxState = reader.read<HurtboxState>(0x34);

		// version < 2.1.0
		if (size <= 0x35) {
			return post;
		}
		post.velocities = {
			.selfInducedAirX = reader.read<float>(0x35),
			.selfInducedY = reader.read<float>(0x39),
			.knockback = {
				.x = reader.read<float>(0x3D),
				.y = reader.read<float>(0x41),
			}, 
			.selfInducedGroundX = reader.read<float>(0x45),
		};
		
		// version < 3.5.0
		if (size <= 0x49) {
			return post;
		}
		post.hitlagRemaining = reader.read<float>(0x49);

		// version < 3.11.0
		if (size <= 0x4D) {
			return post;
		}
		post.animationIndex = reader.read<uint32_t>(0x4D);
		return post;
	}

	Item parseItem(BlockReader& reader, size_t size) {
		Item item;
		item.frameNumber = reader.read<int32_t>(0x1);
		item.type = reader.read<ItemType>(0x5);
		item.state = reader.read<uint8_t>(0x7);
		item.direction = reader.read<float>(0x8);
		item.velocity = {
			.x = reader.read<float>(0xC),
			.y = reader.read<float>(0x10),
		};
		item.position = {
			.x = reader.read<float>(0x14),
			.y = reader.read<float>(0x18),
		};
		item.damage = reader.read<uint16_t>(0x1C);
		item.expirationTimer = reader.read<float>(0x1E);
		item.spawnId = reader.read<uint32_t>(0x22);

		// version < 3.2.0
		if (size <= 0x26) {
			return item;
		}

		item.misc1 = reader.read<uint8_t>(0x26);
		item.misc2 = reader.read<uint8_t>(0x27);
		item.misc3 = reader.read<uint8_t>(0x28);
		item.misc4 = reader.read<uint8_t>(0x29);

		// version < 3.6.0
		if (size <= 0x2A) {
			return item;
		}
		item.owner = reader.read<Port>(0x2A);
		return item;
	}

	Frame::End parseFrameEnd(BlockReader& reader, size_t size) {
		Frame::End end;
		end.frameNumber = reader.read<int32_t>(0x1);

		if (size <= 0x5) {
			return end;
		}
		end.latestFinalizedFrame = reader.read<int32_t>(0x5);
		return end;
	}

	std::unique_ptr<FrameEventProcessor> frameEventProcessor_;
	std::variant<std::monostate, GeckoCodesParser> splitMessageParser_;
	Options options_;
	ShiftJisConverter shiftJis_;
	Handlers& handlers_;
	const std::unordered_map<EventType, uint16_t> payloadSizes_;
	std::optional<uint32_t> numFramesFromMetadata_;
	Reader& stream_;
};

}  // namespace


// Parse a replay from an input stream using the given event handlers.
void Parser::parseGame(Handlers& handlers, reading::Reader& reader) {
	// For speed, assume the `raw` element comes first and handle it manually.
	// The official JS parser does this too, so it should be reliable.
	reader.expectBytes("{U\x03raw[$U#l");

	auto rawLength = reader.read<uint32_t>();

	auto [bytesRead, payloadSizes] = parseEventPayloads(reader);

	std::optional<uint32_t> remainingLength = rawLength > 0 ? std::make_optional(rawLength - bytesRead) : std::nullopt;

	// For Rollback::FIRST and Rollback::SECOND, we can get the exact number of
	// frames by reading the metadata first. We get an estimate for
	// Rollback::ALL elsewhere.
	std::optional<uint32_t> numFramesFromMetadata;
	std::optional<nlohmann::json> rawMetadata; 
	if (options_.rollback != Options::Rollback::ALL) {
		rawMetadata = skipReadMetadata(reader, remainingLength, payloadSizes);
		if (rawMetadata) {
			auto lastFrame = tryGet<int32_t>(*rawMetadata, "lastFrame");
			if (lastFrame) {
				// [kFirstFrame, lastFrame] are inclusive, so we need to add one more.
				numFramesFromMetadata = *lastFrame + 1 - Frame::kFirstFrameNumber;
			}
		}
	}

	EventParser eventParser(options_, handlers, payloadSizes, numFramesFromMetadata, reader);
	eventParser.parseEvents(remainingLength);

	if (options_.interestedEvents & Options::InterestedEvents::METADATA) {
		if (!rawMetadata) {
			rawMetadata = readMetadata(reader);
		}
		Metadata metadata = parseMetadata(*rawMetadata);
		handlers.metadata(metadata, *rawMetadata);
	}
	handlers.finalize();
}

}  // namespace cpp_slippi::parsing

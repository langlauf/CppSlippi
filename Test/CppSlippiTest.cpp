#include "CppSlippi/CppSlippi.h"

#include <array>
#include <filesystem>
#include <format>
#include <stdexcept>

#include <gmock/gmock-matchers.h>
#include <gmock/gmock-more-matchers.h>
#include <gtest/gtest.h>

#include "CppSlippi/Model.h"
#include "CppSlippi/Reading/BufferedReader.h"
#include "CppSlippi/Reading/FileReader.h"
#include "CppSlippi/Reading/Reader.h"
#include "Model/Matchers.h"


using testing::Eq;
using testing::Field;
using testing::Ge;
using testing::IsEmpty;
using testing::Not;
using testing::Optional;
using testing::Pointer;
using testing::SizeIs;
using testing::StrEq;


namespace cpp_slippi {

template<typename Fn>
class Defer {
public:
	Defer(Fn fn) : fn(fn) {}
	~Defer() { fn(); }
private:
	Fn fn;
};

Game parseGame(const char* filename, Options options = {}) {
	FILE* file;
	errno_t err = fopen_s(&file, filename, "rb");
	if (err) {
		const std::string& currentDir = std::filesystem::current_path().string();
		throw std::runtime_error(std::format("Failed to open file {}. Error: {}. Working directory: {}",
			filename, errno, currentDir));
	}
	Defer defer([&] { fclose(file); });

	reading::BufferedReader reader(reading::FileReader{file});
	return game(reader, options);
}

TEST(CppSlippi, OldVersion) {
	Game game = parseGame("v0.1.slp");

	EXPECT_EQ(game.start->slippi.version, Slippi::Version({0, 1, 0}));

	EXPECT_THAT(game.start->players[Port::P1],
		Optional(Field("character", &Player::character, Eq(CssCharacter::FOX))));
	EXPECT_THAT(game.start->players[Port::P2],
		Optional(Field("character", &Player::character, Eq(CssCharacter::GANONDORF))));
	EXPECT_EQ(game.start->players[Port::P3], std::nullopt);
	EXPECT_EQ(game.start->players[Port::P4], std::nullopt);
}

TEST(CppSlippi, BasicGame) {
	Game game = parseGame("game.slp");

	Metadata expectedMetadata = {
		.startAt = "2018-06-22T07:52:59Z",
		.lastFrame = 5085,
		.players = {{
			Metadata::Player {
				.characters = {{InGameCharacter::MARTH, 5209}},
				.names = {},
			},
			Metadata::Player {
				.characters = {{InGameCharacter::FOX, 5209}},
				.names = {},
			},
			std::nullopt,
			std::nullopt,
		}},
		.playedOn = "dolphin",
	};

	Game::Start expectedStart = {
		.slippi = {
			.version = {1, 0, 0, 0},
		},
		.bitfield = static_cast<Game::Bitfield>(0x00060010)  // Other bits.
			| Game::Bitfield::TIMER_COUNT_DOWN
			| Game::Bitfield::MODE_STOCK
			| Game::Bitfield::FRIENDLY_FIRE
			| Game::Bitfield::PAUSE_MODE
			| Game::Bitfield::LRAS_UI_DURING_PAUSE
			| Game::Bitfield::LRAS_ZRETRY_UI_HIDDEN_DURING_PAUSE
			| Game::Bitfield::ANALOG_STICK_UI_DURING_PAUSE,
		.timerBehavior = TimerBehavior::COUNT_DOWN,
		.gameMode = GameMode::STOCK,
		.isRainingBombs = false,
		.isTeams = false,
		.itemSpawnFrequency = ItemSpawnFrequency::OFF,
		.selfDestructScore = -1,
		.stage = Stage::YOSHIS_STORY,
		.timer = 480,
		.itemSpawns = ItemSpawns::ALL,
		.damageRatio = 1.0,
		.players = {
			Player {
				.character = CssCharacter::MARTH,
				.type = Player::Type::HUMAN,
				.stocks = 4,
				.costume = static_cast<Costume>(3),
				.handicap = 9,
				.bitfield = Player::Bitfield::START_ON_WARP_IN_PLATFORM | Player::Bitfield::RUMBLE_ENABLED,
				.cpuLevel = 1,
				.offenseRatio = 1.0,
				.defenseRatio = 1.0,
				.modelScale = 1.0,
				.ucf = Ucf{
					.dashBack = Ucf::DashBack::OFF,
					.shieldDrop = Ucf::ShieldDrop::OFF,
				},
			},
			Player {
				.character = CssCharacter::FOX,
				.type = Player::Type::CPU,
				.stocks = 4,
				.costume = static_cast<Costume>(0),
				.handicap = 9,
				.bitfield = Player::Bitfield::START_ON_WARP_IN_PLATFORM,
				.cpuLevel = 1,
				.offenseRatio = 1.0,
				.defenseRatio = 1.0,
				.modelScale = 1.0,
				.ucf = Ucf {
					.dashBack = Ucf::DashBack::OFF,
					.shieldDrop = Ucf::ShieldDrop::OFF,
				},
			},
		},
		.randomSeed = 3803194226,
	};


	Game::End expectedEnd = {
		.method = Game::End::Method::CONCLUSIVE,
	};

	EXPECT_THAT(game.start, Optional(GameStartEq(expectedStart)));
	EXPECT_THAT(game.frames, SizeIs(5209));
	EXPECT_THAT(game.end, Optional(GameEndEq(expectedEnd)));
	EXPECT_THAT(game.metadata, Optional(MetadataEq(expectedMetadata)));
}

TEST(CppSlippi, StreamingGame) {
	Game game = parseGame("streaming_game.slp");

	Metadata expectedMetadata = {
		.startAt = "2018-06-22T07:52:59Z",
		.lastFrame = 5085,
		.players = {{
			Metadata::Player {
				.characters = {{InGameCharacter::MARTH, 5209}},
				.names = {},
			},
			Metadata::Player {
				.characters = {{InGameCharacter::FOX, 5209}},
				.names = {},
			},
			std::nullopt,
			std::nullopt,
		}},
		.playedOn = "dolphin",
	};

	Game::Start expectedStart = {
		.slippi = {
			.version = {1, 0, 0, 0},
		},
		.bitfield = static_cast<Game::Bitfield>(0x00060010)  // Other bits.
			| Game::Bitfield::TIMER_COUNT_DOWN
			| Game::Bitfield::MODE_STOCK
			| Game::Bitfield::FRIENDLY_FIRE
			| Game::Bitfield::PAUSE_MODE
			| Game::Bitfield::LRAS_UI_DURING_PAUSE
			| Game::Bitfield::LRAS_ZRETRY_UI_HIDDEN_DURING_PAUSE
			| Game::Bitfield::ANALOG_STICK_UI_DURING_PAUSE,
		.timerBehavior = TimerBehavior::COUNT_DOWN,
		.gameMode = GameMode::STOCK,
		.isRainingBombs = false,
		.isTeams = false,
		.itemSpawnFrequency = ItemSpawnFrequency::OFF,
		.selfDestructScore = -1,
		.stage = Stage::YOSHIS_STORY,
		.timer = 480,
		.itemSpawns = ItemSpawns::ALL,
		.damageRatio = 1.0,
		.players = {
			Player {
				.character = CssCharacter::MARTH,
				.type = Player::Type::HUMAN,
				.stocks = 4,
				.costume = static_cast<Costume>(3),
				.handicap = 9,
				.bitfield = Player::Bitfield::START_ON_WARP_IN_PLATFORM | Player::Bitfield::RUMBLE_ENABLED,
				.cpuLevel = 1,
				.offenseRatio = 1.0,
				.defenseRatio = 1.0,
				.modelScale = 1.0,
				.ucf = Ucf{
					.dashBack = Ucf::DashBack::OFF,
					.shieldDrop = Ucf::ShieldDrop::OFF,
				},
			},
			Player {
				.character = CssCharacter::FOX,
				.type = Player::Type::CPU,
				.stocks = 4,
				.costume = static_cast<Costume>(0),
				.handicap = 9,
				.bitfield = Player::Bitfield::START_ON_WARP_IN_PLATFORM,
				.cpuLevel = 1,
				.offenseRatio = 1.0,
				.defenseRatio = 1.0,
				.modelScale = 1.0,
				.ucf = Ucf {
					.dashBack = Ucf::DashBack::OFF,
					.shieldDrop = Ucf::ShieldDrop::OFF,
				},
			},
		},
		.randomSeed = 3803194226,
	};
	
	Game::End expectedEnd = {
		.method = Game::End::Method::CONCLUSIVE,
	};

	EXPECT_THAT(game.start, Optional(GameStartEq(expectedStart)));
	EXPECT_THAT(game.frames, SizeIs(5209));
	EXPECT_THAT(game.end, Optional(GameEndEq(expectedEnd)));
	EXPECT_THAT(game.metadata, Optional(MetadataEq(expectedMetadata)));
}

TEST(CppSlippi, IceClimbers) {
	Game game = parseGame("ics.slp");

	Metadata::Player expected = Metadata::Player {
		.characters = {
			{InGameCharacter::NANA, 344},
			{InGameCharacter::POPO, 344},
		},
		.names = {},
	};

	ASSERT_TRUE(game.metadata->players.has_value());
	EXPECT_THAT(game.metadata->players->at(0), Optional(MetadataPlayerEq(expected)));

	ASSERT_TRUE(game.start->players[Port::P1].has_value());
	EXPECT_THAT(game.start->players[Port::P1]->character, CssCharacter::ICE_CLIMBERS);

	ASSERT_THAT(game.frames, Not(IsEmpty()));
	ASSERT_TRUE(game.frames[0].ports[Port::P1].has_value());
	EXPECT_TRUE(game.frames[0].ports[Port::P1]->follower.has_value());
}

TEST(CppSlippi, Ucf) {
	Game shieldDrop = parseGame("shield_drop.slp");
	ASSERT_TRUE(shieldDrop.start->players[Port::P1].has_value());
	EXPECT_THAT(shieldDrop.start->players[Port::P1]->ucf,
		Optional(UcfEq({Ucf::DashBack::OFF, Ucf::ShieldDrop::UCF})));

	Game dashBack = parseGame("dash_back.slp");
	ASSERT_TRUE(dashBack.start->players[Port::P1].has_value());
	EXPECT_THAT(dashBack.start->players[Port::P1]->ucf,
		Optional(UcfEq({Ucf::DashBack::UCF, Ucf::ShieldDrop::OFF})));
}

TEST(CppSlippi, ButtonsLzrs) {
	Game game = parseGame("buttons_lrzs.slp");

	ASSERT_THAT(game.frames, SizeIs(Ge(1049)));
	EXPECT_THAT(game.frames[151].ports[Port::P1]->leader.pre->buttons,
		ButtonsEq({Buttons::Logical::TRIGGER,
		           Buttons::Physical::NONE}));
	EXPECT_THAT(game.frames[172].ports[Port::P1]->leader.pre->buttons,
		ButtonsEq({Buttons::Logical::TRIGGER | Buttons::Logical::L,
		           Buttons::Physical::L}));
	EXPECT_THAT(game.frames[535].ports[Port::P1]->leader.pre->buttons,
		ButtonsEq({Buttons::Logical::TRIGGER,
		           Buttons::Physical::NONE}));
	EXPECT_THAT(game.frames[571].ports[Port::P1]->leader.pre->buttons,
		ButtonsEq({Buttons::Logical::TRIGGER | Buttons::Logical::R,
		           Buttons::Physical::R}));
	EXPECT_THAT(game.frames[929].ports[Port::P1]->leader.pre->buttons,
		ButtonsEq({Buttons::Logical::TRIGGER | Buttons::Logical::A | Buttons::Logical::Z,
		           Buttons::Physical::Z}));
	EXPECT_THAT(game.frames[1049].ports[Port::P1]->leader.pre->buttons,
		ButtonsEq({Buttons::Logical::START,
		           Buttons::Physical::START}));
}

TEST(CppSlippi, ButtonsAbxy) {
	Game game = parseGame("buttons_abxy.slp");

	Port p = Port::P1;

	ASSERT_THAT(game.frames, SizeIs(Ge(325)));
	EXPECT_THAT(game.frames[124].ports[Port::P1]->leader.pre->buttons,
		ButtonsEq({Buttons::Logical::A, Buttons::Physical::A}));
	EXPECT_THAT(game.frames[169].ports[Port::P1]->leader.pre->buttons,
		ButtonsEq({Buttons::Logical::B, Buttons::Physical::B}));
	EXPECT_THAT(game.frames[244].ports[Port::P1]->leader.pre->buttons,
		ButtonsEq({Buttons::Logical::X, Buttons::Physical::X}));
	EXPECT_THAT(game.frames[325].ports[Port::P1]->leader.pre->buttons,
		ButtonsEq({Buttons::Logical::Y, Buttons::Physical::Y}));
}

TEST(CppSlippi, DpadUdlr) {
	Game game = parseGame("dpad_udlr.slp");

	ASSERT_THAT(game.frames, SizeIs(Ge(347)));
	EXPECT_THAT(game.frames[150].ports[Port::P1]->leader.pre->buttons,
		ButtonsEq({Buttons::Logical::DPAD_UP, Buttons::Physical::DPAD_UP}));
	EXPECT_THAT(game.frames[233].ports[Port::P1]->leader.pre->buttons,
		ButtonsEq({Buttons::Logical::DPAD_DOWN, Buttons::Physical::DPAD_DOWN}));
	EXPECT_THAT(game.frames[292].ports[Port::P1]->leader.pre->buttons,
		ButtonsEq({Buttons::Logical::DPAD_LEFT, Buttons::Physical::DPAD_LEFT}));
	EXPECT_THAT(game.frames[347].ports[Port::P1]->leader.pre->buttons,
		ButtonsEq({Buttons::Logical::DPAD_RIGHT, Buttons::Physical::DPAD_RIGHT}));
}

TEST(CppSlippi, CStickUdlr) {
	Game game = parseGame("cstick_udlr.slp");

	ASSERT_THAT(game.frames, SizeIs(Ge(367)));
	EXPECT_THAT(game.frames[137].ports[Port::P1]->leader.pre->buttons,
		ButtonsEq({Buttons::Logical::CSTICK_UP, Buttons::Physical::NONE}));
	EXPECT_THAT(game.frames[219].ports[Port::P1]->leader.pre->buttons,
		ButtonsEq({Buttons::Logical::CSTICK_DOWN, Buttons::Physical::NONE}));
	EXPECT_THAT(game.frames[301].ports[Port::P1]->leader.pre->buttons,
		ButtonsEq({Buttons::Logical::CSTICK_LEFT, Buttons::Physical::NONE}));
	EXPECT_THAT(game.frames[367].ports[Port::P1]->leader.pre->buttons,
		ButtonsEq({Buttons::Logical::CSTICK_RIGHT, Buttons::Physical::NONE}));
}

TEST(CppSlippi, JoystickUdlr) {
	Game game = parseGame("joystick_udlr.slp");

	ASSERT_THAT(game.frames, SizeIs(Ge(296)));
	EXPECT_THAT(game.frames[125].ports[Port::P1]->leader.pre->buttons,
		ButtonsEq({Buttons::Logical::JOYSTICK_UP, Buttons::Physical::NONE}));
	EXPECT_THAT(game.frames[196].ports[Port::P1]->leader.pre->buttons,
		ButtonsEq({Buttons::Logical::JOYSTICK_DOWN, Buttons::Physical::NONE}));
	EXPECT_THAT(game.frames[246].ports[Port::P1]->leader.pre->buttons,
		ButtonsEq({Buttons::Logical::JOYSTICK_LEFT, Buttons::Physical::NONE}));
	EXPECT_THAT(game.frames[296].ports[Port::P1]->leader.pre->buttons,
		ButtonsEq({Buttons::Logical::JOYSTICK_RIGHT, Buttons::Physical::NONE}));
}

TEST(CppSlippi, Nintendont) {
	Game game = parseGame("nintendont.slp");

	EXPECT_THAT(game.metadata->playedOn, Optional(testing::StrEq("nintendont")));
}

TEST(CppSlippi, Names) {
	Game game = parseGame("netplay.slp");

	std::unordered_map<std::string, std::string> p1Names = {{"code", "ABCD#123"}, {"netplay", "abcdefghijk"}};
	std::unordered_map<std::string, std::string> p2Names = {{"code", "XX#000"}, {"netplay", "nobody"}};

	ASSERT_TRUE(game.metadata->players.has_value());

	ASSERT_TRUE(game.metadata->players->at(0).has_value());
	EXPECT_EQ(game.metadata->players->at(0)->names, p1Names);

	ASSERT_TRUE(game.metadata->players->at(1).has_value());
	EXPECT_EQ(game.metadata->players->at(1)->names, p2Names);
}

TEST(CppSlippi, ConsoleName) {
	Game game = parseGame("console_name.slp");

	EXPECT_THAT(game.metadata->consoleNick, Optional(testing::StrEq("Station 1")));
}

TEST(CppSlippi, v2) {
	Game game = parseGame("v2.0.slp");

	EXPECT_EQ(game.start->slippi.version, Slippi::Version({2, 0, 1}));
}

TEST(CppSlippi, v3_12) {
	Game game = parseGame("v3.12.slp", { .fullwidthToHalfwidth = true });

	Game::Start expectedStart = {
		.slippi = {
			.version = {3, 12, 0, 0},
		},
		.bitfield = static_cast<Game::Bitfield>(0x000E0010)  // Other bits.
			| Game::Bitfield::TIMER_COUNT_DOWN
			| Game::Bitfield::MODE_STOCK
			| Game::Bitfield::FRIENDLY_FIRE
			| Game::Bitfield::PAUSE_MODE
			| Game::Bitfield::LRAS_UI_DURING_PAUSE
			| Game::Bitfield::LRAS_ZRETRY_UI_HIDDEN_DURING_PAUSE
			| Game::Bitfield::ANALOG_STICK_UI_DURING_PAUSE,
		.timerBehavior = TimerBehavior::COUNT_DOWN,
		.gameMode = GameMode::STOCK,
		.isRainingBombs = false,
		.isTeams = false,
		.itemSpawnFrequency = ItemSpawnFrequency::OFF,
		.selfDestructScore = -1,
		.stage = Stage::POKEMON_STADIUM,
		.timer = 480, 
		.itemSpawns = ItemSpawns::ALL,
		.damageRatio = 1.0,
		.players = {
			Player {
				.character = CssCharacter::MARTH,
				.type = Player::Type::HUMAN,
				.stocks = 4,
				.costume = static_cast<Costume>(3),
				.team = std::nullopt,
				.handicap = 9,
				.bitfield = Player::Bitfield::RUMBLE_ENABLED | Player::Bitfield::START_ON_WARP_IN_PLATFORM,
				.cpuLevel = 1,
				.offenseRatio = 1.0,
				.defenseRatio = 1.0,
				.modelScale = 1.0,
				.ucf = Ucf {
					.dashBack = Ucf::DashBack::UCF,
					.shieldDrop = Ucf::ShieldDrop::UCF
				},
				.tag = "",
				.netplay = Netplay {
					.name = "xxxxxx",
					.code = "XX#111",
					.slippiUid = "aaaaaaaaaaaaaaaaaaaaaaaaaaaa",
				},
			},
			Player {
				.character = CssCharacter::MARTH,
				.type = Player::Type::HUMAN,
				.stocks = 4,
				.costume = static_cast<Costume>(0),
				.team = std::nullopt,
				.handicap = 9,
				.bitfield = Player::Bitfield::RUMBLE_ENABLED | Player::Bitfield::START_ON_WARP_IN_PLATFORM,
				.cpuLevel = 1,
				.offenseRatio = 1.0,
				.defenseRatio = 1.0,
				.modelScale = 1.0,
				.ucf = Ucf {
					.dashBack = Ucf::DashBack::UCF,
					.shieldDrop = Ucf::ShieldDrop::UCF,
				},
				.tag = "",
				.netplay = Netplay {
					.name = "yyyyyyyyyy",
					.code = "YYYY#222",
					.slippiUid = "bbbbbbbbbbbbbbbbbbbbbbbbbbbb",
				},
			},
		},
		.randomSeed = 39656,
		.isPal = false,
		.isFrozenPS = false,
		.scene = Scene {
			.minor = 2,
			.major = 8,
		},
		.language = Language::ENGLISH,
	};

	EXPECT_THAT(game.start, Optional(GameStartEq(expectedStart)));
}

TEST(CppSlippi, CorruptNames_ParsedSafely) {
	// P1's names in this file are all missing null-terminators.
	Game game = parseGame("corrupt_names.slp", { .fullwidthToHalfwidth = true });

	Netplay netplay = {
		.name = "abcdefghijklmnopqrstuvwxyzabcd",
		.code = "XX#11123",
		.slippiUid = "abcdefghijklmnopqrstuvwxyzab",
	};

	EXPECT_THAT(*(game.start->players[Port::P1]->tag), testing::StrEq("abcdefghijklmnop"));
	EXPECT_THAT(*(game.start->players[Port::P1]->netplay), NetplayEq(netplay));
}

TEST(CppSlippi, UnknownEvent_DoesNotThrowException) {
	Game game = parseGame("unknown_event.slp");
}

TEST(CppSlippi, ZeldaTransformation) {
	Game game = parseGame("transform.slp");

	ASSERT_THAT(game.frames, SizeIs(Ge(400)));
	ActionState state = game.frames[400].ports[Port::P4]->leader.pre->state;
	EXPECT_THAT(ActionMapper::get(InGameCharacter::ZELDA).toAction(state), Action::TRANSFORM_GROUND);
}

TEST(CppSlippi, Items) {
	Game game = parseGame("items.slp");


	ASSERT_THAT(game.frames, SizeIs(Ge(503)));
	EXPECT_THAT(game.frames[121].items.at(0), ItemEq({
			.frameNumber = -2,
			.type = ItemType::PEACH_TURNIP,
			.state = 0,
			.direction = 1.0,
			.velocity = {
				.x = 0.0,
				.y = 0.0,
			},
			.position = {
				.x = -62.709'606f,
				.y = -1.493'274'9f,
			},
			.damage = 0,
			.expirationTimer = 140.0,
			.spawnId = 0,
			.misc1 = 5,
			.misc2 = 5,
			.misc3 = 5,
			.misc4 = 5,
			.owner = Port::P1,
		}));
	EXPECT_THAT(game.frames[275].items.at(0), ItemEq({
			.frameNumber = 152,
			.type = ItemType::PEACH_TURNIP,
			.state = 0,
			.direction = -1.0,
			.velocity = {
				.x = 0.0,
				.y = 0.0,
			},
			.position = {
				.x = 20.395'56f,
				.y = -1.493'274'9f,
			},
			.damage = 0,
			.expirationTimer = 140.0,
			.spawnId = 1,
			.misc1 = 5,
			.misc2 = 0,
			.misc3 = 5,
			.misc4 = 5,
			.owner = Port::P1,
		}));
	EXPECT_THAT(game.frames[503].items.at(0), ItemEq({
			.frameNumber = 380,
			.type = ItemType::PEACH_TURNIP,
			.state = 0,
			.direction = 1.0,
			.velocity = {
				.x = 0.0,
				.y = 0.0,
			},
			.position = {
				.x = -3.982'539'2f,
				.y = -1.493'274'9f,
			},
			.damage = 0,
			.expirationTimer = 140.0,
			.spawnId = 2,
			.misc1 = 5,
			.misc2 = 0,
			.misc3 = 5,
			.misc4 = 5,
			.owner = Port::P1,
		}));
}

TEST(CppSlippi, Rollback) {
	Game gameAll = parseGame("ics2.slp", { .rollback = Options::Rollback::ALL });
	Game gameFirst = parseGame("ics2.slp", { .rollback = Options::Rollback::FIRST });
	Game gameLast = parseGame("ics2.slp", { .rollback = Options::Rollback::LAST });

	ASSERT_THAT(gameAll.frames, SizeIs(9530));
	ASSERT_THAT(gameFirst.frames, SizeIs(9519));
	ASSERT_THAT(gameLast.frames, SizeIs(9519));

	EXPECT_THAT(gameAll.frames[474], FrameEq(gameFirst.frames[474]));
	EXPECT_THAT(gameAll.frames[475], FrameEq(gameLast.frames[474]));
	EXPECT_THAT(gameAll.frames[476], FrameEq(gameFirst.frames[475]));
	EXPECT_THAT(gameAll.frames[476], FrameEq(gameLast.frames[475]));
}

TEST(CppSlippi, SkipUninterestedEvents) {
	Game game = parseGame("items.slp", { .interestedEvents = Options::InterestedEvents::FRAME_PRE });

	EXPECT_THAT(game.start, Eq(std::nullopt));
	EXPECT_THAT(game.end, Eq(std::nullopt));
	EXPECT_THAT(game.metadata, Eq(std::nullopt));

	ASSERT_THAT(game.frames, SizeIs(7375));
	EXPECT_THAT(game.frames[121].ports[0]->leader.pre, Not(Eq(std::nullopt)));
	EXPECT_THAT(game.frames[121].ports[0]->leader.post, Eq(std::nullopt));
	// This frame would normally have an item.
	EXPECT_THAT(game.frames[121].items, IsEmpty());
}

TEST(CppSlippi, SkipToGameEnd) {
	Game game = parseGame("game.slp", { .interestedEvents = Options::InterestedEvents::GAME_END });

	EXPECT_THAT(game.start, Eq(std::nullopt));
	EXPECT_THAT(game.frames, IsEmpty());
	EXPECT_THAT(game.metadata, Eq(std::nullopt));

	Game::End expectedEnd = {
		.method = Game::End::Method::CONCLUSIVE,
	};

	EXPECT_THAT(game.end, Optional(GameEndEq(expectedEnd)));
}

TEST(CppSlippi, SkipFrames) {
	Game game = parseGame("game.slp", { .interestedEvents = Options::InterestedEvents::GAME_EVENTS });

	Game::Start expectedStart = {
		.slippi = {
			.version = {1, 0, 0, 0},
		},
		.bitfield = static_cast<Game::Bitfield>(0x00060010)  // Other bits.
			| Game::Bitfield::TIMER_COUNT_DOWN
			| Game::Bitfield::MODE_STOCK
			| Game::Bitfield::FRIENDLY_FIRE
			| Game::Bitfield::PAUSE_MODE
			| Game::Bitfield::LRAS_UI_DURING_PAUSE
			| Game::Bitfield::LRAS_ZRETRY_UI_HIDDEN_DURING_PAUSE
			| Game::Bitfield::ANALOG_STICK_UI_DURING_PAUSE,
		.timerBehavior = TimerBehavior::COUNT_DOWN,
		.gameMode = GameMode::STOCK,
		.isRainingBombs = false,
		.isTeams = false,
		.itemSpawnFrequency = ItemSpawnFrequency::OFF,
		.selfDestructScore = -1,
		.stage = Stage::YOSHIS_STORY,
		.timer = 480,
		.itemSpawns = ItemSpawns::ALL,
		.damageRatio = 1.0,
		.players = {
			Player {
				.character = CssCharacter::MARTH,
				.type = Player::Type::HUMAN,
				.stocks = 4,
				.costume = static_cast<Costume>(3),
				.handicap = 9,
				.bitfield = Player::Bitfield::START_ON_WARP_IN_PLATFORM | Player::Bitfield::RUMBLE_ENABLED,
				.cpuLevel = 1,
				.offenseRatio = 1.0,
				.defenseRatio = 1.0,
				.modelScale = 1.0,
				.ucf = Ucf{
					.dashBack = Ucf::DashBack::OFF,
					.shieldDrop = Ucf::ShieldDrop::OFF,
				},
			},
			Player {
				.character = CssCharacter::FOX,
				.type = Player::Type::CPU,
				.stocks = 4,
				.costume = static_cast<Costume>(0),
				.handicap = 9,
				.bitfield = Player::Bitfield::START_ON_WARP_IN_PLATFORM,
				.cpuLevel = 1,
				.offenseRatio = 1.0,
				.defenseRatio = 1.0,
				.modelScale = 1.0,
				.ucf = Ucf {
					.dashBack = Ucf::DashBack::OFF,
					.shieldDrop = Ucf::ShieldDrop::OFF,
				},
			},
		},
		.randomSeed = 3803194226,
	};


	Game::End expectedEnd = {
		.method = Game::End::Method::CONCLUSIVE,
	};

	EXPECT_THAT(game.start, Optional(GameStartEq(expectedStart)));
	EXPECT_THAT(game.frames, IsEmpty());
	EXPECT_THAT(game.end, Optional(GameEndEq(expectedEnd)));
	EXPECT_THAT(game.metadata, Eq(std::nullopt));
}

}  // namespace cpp_slippi

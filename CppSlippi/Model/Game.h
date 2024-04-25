#pragma once

#include <bit>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "CppSlippi/Model/Enums/Item.h"
#include "CppSlippi/Model/Enums/Port.h"
#include "CppSlippi/Model/Enums/Stage.h"
#include "CppSlippi/Model/Frame.h"
#include "CppSlippi/Model/Metadata.h"
#include "CppSlippi/Model/Player.h"
#include "CppSlippi/Utils/BitFields.h"
#include "CppSlippi/Utils/Endian.h"
#include "CppSlippi/Utils/PortArray.h"


namespace cpp_slippi {

// Information about the Slippi recorder that generated this replay.
struct Slippi {
	struct Version {
		static constexpr std::endian kEndian = std::endian::little;

		uint8_t major;
		uint8_t minor;
		uint8_t build;
		uint8_t unused;

		friend constexpr auto operator<=>(const Version&, const Version&) = default;
	};

	Version version;

	friend constexpr bool operator==(const Slippi&, const Slippi&) = default;
};


enum class TimerBehavior : uint8_t {
	NONE = 0,
	COUNT_DOWN = 2,
	COUNT_UP = 3,
};


enum class GameMode : uint8_t {
	TIME = 0,
	STOCK = 1,
	COIN = 2,
	BONUS = 3,
};


enum class ItemSpawnFrequency : int8_t {
	OFF = -1,
	VERY_LOW = 0,
	LOW = 1,
	MEDIUM = 2,
	HIGH = 3,
	VERY_HIGH = 4,
};


struct Scene {
	// Mostly useless atm, should always be 0x2
	uint8_t minor;
	// 0x2 when the game is played from VS mode, 0x8 when online game (has
	// rollbacks)
	uint8_t major;

	friend constexpr bool operator==(const Scene&, const Scene&) = default;
};


enum class Language : uint8_t {
	JAPANESE = 0,
	ENGLISH = 1,
};


struct GeckoCodes {
	std::vector<uint8_t> bytes;

	friend constexpr bool operator==(const GeckoCodes&, const GeckoCodes&) = default;
};


struct Game {
	// See https://github.com/project-slippi/slippi-wiki/blob/master/SPEC.md#game-bitfield-1
	enum class Bitfield : uint32_t {
		TIMER_BEHAVIOR_MASK = 0x3,
		CHARACTER_PLACES_IN_UI_MASK = 0x1C,
		GAME_MODE_MASK = 0xE0,

		NO_TIMER = 0x0,
		TIMER_COUNT_DOWN = 0x2,
		TIME_COUNT_UP = 0x3,

		MODE_TIME = 0x0,
		MODE_STOCK = 0x20,
		MODE_COIN = 0x40,
		MODE_BONUS = 0x60,

		NONE = 0u,
		FRIENDLY_FIRE = 1u << 8,
		BTT_OR_TITLE_SCREEN_DEMO = 1u << 9,
		CLASSIC_OR_ADVENTURE_MODE = 1u << 10,
		HRC_OR_EVENT_MATCH = 1u << 11,
		ALL_STAR_IN_GAME_OR_WAITING_1 = 1u << 12,
		ALL_STAR_IN_GAME_OR_WAITING_2 = 1u << 13,
		ALL_STAR_IN_GAME_3 = 1u << 14,
		ALL_STAR_IN_GAME_4 = 1u << 15,

		SINGLE_BUTTON_MODE = 1u << 20,
		PAUSE_MODE = 1u << 23,

		TIMER_UI_DURING_PAUSE = 1u << 24,
		HUD_DURING_PAUSE = 1u << 25,
		LRAS_UI_DURING_PAUSE = 1u << 26,
		LRAS_ZRETRY_UI_HIDDEN_DURING_PAUSE = 1u << 27,
		ZRETRY_UI_DURING_PAUSE = 1u << 28,
		ANALOG_STICK_UI_DURING_PAUSE = 1u << 30,
		SCORE_DISPLAY_UI_DURING_PAUSE = 1u << 31,
	};

	// Information used to initialize the game such as the game mode, settings,
	// characters & stage.
	struct Start {
		// Information about the Slippi recorder that generated this replay
		Slippi slippi;

		// Contains misc game information
		Bitfield bitfield;

		// Timer behavior
		TimerBehavior timerBehavior;

		// Game mode
		GameMode gameMode;

		// Value is false if bomb rain is disabled. Any other value will cause bombs
		// to start dropping after 20 seconds have elapsed.
		bool isRainingBombs;

		// True if this was a teams game
		bool isTeams;

		// Indicates how frequently items spawn
		ItemSpawnFrequency itemSpawnFrequency;

		// Indicates how an SD should be interpreted for scoring. Can be -2, -1, or 0
		// if set by the game
		int8_t selfDestructScore;

		// Stage on which this game was played
		Stage stage;

		// The number of seconds for the timer. Will be specified in this field
		// regardless of game mode
		uint32_t timer;

		// Item spawns
		ItemSpawns itemSpawns;

		// Damage ratio
		float damageRatio;

		// Players in this game by port (port 1 is at index 0; empty ports will
		// contain std::nullopt)
		utils::PortArray<std::optional<Player>> players;

		// The random seed before the game start
		uint32_t randomSeed;

		// added(1.5.0) True if this was a PAL version of Melee
		std::optional<bool> isPal;

		// added(2.0.0) True if frozen Pokemon Stadium was enabled
		std::optional<bool> isFrozenPS;

		// added(3.7.0) Scene
		std::optional<Scene> scene;

		// added(3.12.0) Needed for HRC because stage is different between the
		// languages
		std::optional<Language> language;

		// added(3.14.0) An ID consisting of the mode and time the match started
		// (e.g. mode.unranked-2022-12-20T06:52:39.18-0). Max 50 characters +
		// null terminator
		std::optional<std::string> matchId;

		// added(3.14.0) For the given Match ID, starts at 1
		std::optional<uint32_t> gameNumber;

		// added(3.14.0) For the given Game Number, will be 0 if not a tiebreak
		// game
		std::optional<uint32_t> tiebreakerNumber;

		friend constexpr bool operator==(const Start&, const Start&) = default;
	};

	// Information about the end of the game.
	struct End {
		enum class Method : uint8_t {
			// obsoleted(2.0.0)
			INCONCLUSIVE = 0,
			// added(2.0.0)
			TIME = 1,
			// added(2.0.0)
			GAME = 2,
			// obsoleted(2.0.0)
			CONCLUSIVE = 3,
			// added(2.0.0)
			NO_CONTEST = 7,
		};

		// changed(2.0.0) How the game ended
		Method method;
		// added(2.0.0) Index of player that LRAS'd, if any
		std::optional<Port> lrasInitiator;
		// added(3.13.0) 0-indexed placement positions. -1 if player not in game
		std::optional<utils::PortArray<int8_t>> playerPlacements;
	};

	std::optional<Start> start;
	std::optional<End> end;
	std::vector<Frame> frames;
	std::optional<Metadata> metadata;
	std::optional<nlohmann::json> rawMetadata;
	std::optional<GeckoCodes> geckoCodes;
};

template<> inline constexpr bool is_bitfield<Game::Bitfield> = true;
template<> inline constexpr std::endian kEndian<Game::Bitfield> = std::endian::little;

}  // namespace cpp_slippi

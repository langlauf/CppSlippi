#pragma once

#include <cstdint>
#include <optional>
#include <string>

#include "CppSlippi/Model/Enums/Character.h"
#include "CppSlippi/Model/Enums/Costume.h"
#include "CppSlippi/Utils/BitFields.h"


namespace cpp_slippi {

struct Team {
	enum class Color : uint8_t {
		RED = 0,
		BLUE = 1,
		GREEN = 2,
	};

	enum class Shade : uint8_t {
		NORMAL = 0,
		LIGHT = 1,
		DARK = 2,
	};

	// Team color
	Color color;

	// Indicates coloration changes for multiples of the same character on
	// the same team.
	Shade shade;

	friend constexpr bool operator==(const Team&, const Team&) = default;
};


// Information about the "Universal Controller Fix" mod.
struct Ucf {
	enum class DashBack : uint32_t {
		OFF = 0,
		UCF = 1,
		ARDUINO = 2,
	};

	enum class ShieldDrop : uint32_t {
		OFF = 0,
		UCF = 1,
		ARDUINO = 2,
	};

	// UCF dashback status
	DashBack dashBack;

	// UCF shield drop status
	ShieldDrop shieldDrop;

	friend constexpr bool operator==(const Ucf&, const Ucf&) = default;
};


// Netplay name, connect code, and Slippi UID.
struct Netplay {
	// Display name used by the player
	std::string name;

	// Connect code used by the player
	std::string code;

	// added(3.11.0) Slippi UID
	std::optional<std::string> slippiUid;

	friend constexpr bool operator==(const Netplay&, const Netplay&) = default;
};


// Information about each player such as character, team, stock count, etc.
struct Player {
	enum class Type : uint8_t {
		HUMAN = 0,
		CPU = 1,
		DEMO = 2,
		EMPTY = 3,
	};

	enum class Bitfield : uint8_t {
		NONE = 0,
		STAMINA = 1 << 0,
		SILENT_CHARACTER = 1 << 1,
		LOW_GRAVITY = 1 << 2,
		INVISIBLE = 1 << 3,
		BLACK_STOCK_ICON = 1 << 4,
		METAL = 1 << 5,
		START_ON_WARP_IN_PLATFORM = 1 << 6,
		RUMBLE_ENABLED = 1 << 7,
	};

	// Character selected
	CssCharacter character;

	// Player type(human/cpu)
	Type type;

	// Starting stock count
	uint8_t stocks;

	// Costume ID
	Costume costume;

	// Team, if this was a teams game
	std::optional<Team> team;

	// The handicap set on the player. Will affect offense/defense ratios later
	// in the player record
	uint8_t handicap;

	// Player bitfield
	Bitfield bitfield;

	// Indicates what the CPU level is. Still specified on human players
	uint8_t cpuLevel;

	// This is the percent the fighter's first stock will start at
	uint16_t damageStart;

	// This is the percent the fighter's stocks will start at. This value will
	// be used for first stock as well if Damage Start is 0
	uint16_t damageSpawn;

	// Indicates a knockback multiplier when this player hits another
	float offenseRatio;

	// Indicates a knockback multiplier when this player is hit
	float defenseRatio;

	// Indicates a multiplier on the size scaling of the character's model
	float modelScale;

	// added(1.0.0) UCF feature toggles
	std::optional<Ucf> ucf;

	// added(1.3.0) Name tag
	std::optional<std::string> tag;

	// added(3.9.0) Netplay info
	std::optional<Netplay> netplay;

	friend constexpr bool operator==(const Player&, const Player&) = default;
};
template<> inline constexpr bool is_bitfield<Player::Bitfield> = true;

}  // namespace cpp_slippi

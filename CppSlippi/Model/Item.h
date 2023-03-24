#pragma once

#include <cstdint>
#include <optional>

#include "CppSlippi/Model/Enums/Item.h"
#include "CppSlippi/Model/Enums/Port.h"
#include "CppSlippi/Model/Primitives.h"


namespace cpp_slippi {

// An active item (includes projectiles).
struct Item {
	// The number of the frame. Starts at -123. Frame 0 is when the timer starts
	// counting down
	int32_t frameNumber;

	// Item type
	ItemType type;

	// The state the item is in. Mostly undocumented, might differ per type
	uint8_t state;

	// Direction item is facing, -1 = facing left, +1 = facing right
	float direction;

	// Item's velocity
	Velocity velocity;

	// Item's position
	Position position;

	// Amount of damage item has taken
	uint16_t damage;

	// Number of frames remaining before item expires. Can go into the negatives
	// for certain items such as Link arrows
	float expirationTimer;

	// Auto-incremented number whenever an item spawns: 0, 1, 2, 3, etc
	uint32_t spawnId;

	// added(3.2.0) Samus missile type
	uint8_t misc1;

	// added(3.2.0) Peach turnip face
	uint8_t misc2;

	// added(3.2.0) Samus/Mewtwo isLaunched boolean for charge shot
	uint8_t misc3;

	// added(3.2.0) Samus/Mewtwo current charged power
	uint8_t misc4;

	// added(3.6.0) Port of player who owns this item. -1 when not owned
	std::optional<Port> owner;

	friend bool operator==(const Item&, const Item&) = default;
};

}  // namespace cpp_slippi

#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <optional>
#include <variant>
#include <vector>

#include "CppSlippi/Model/Buttons.h"
#include "CppSlippi/Model/Enums/ActionState.h"
#include "CppSlippi/Model/Enums/Attack.h"
#include "CppSlippi/Model/Enums/Character.h"
#include "CppSlippi/Model/Enums/Port.h"
#include "CppSlippi/Model/Item.h"
#include "CppSlippi/Model/Triggers.h"
#include "CppSlippi/Model/Primitives.h"


namespace cpp_slippi {

// See https://github.com/project-slippi/slippi-wiki/blob/master/SPEC.md#state-bit-flags-1
enum class StateFlags : uint64_t {
	REFLECT = 1llu << 4,
	UNTOUCHABLE = 1llu << 10,
	FAST_FALL = 1llu << 11,
	HIT_LAG = 1llu << 13,
	SHIELD = 1llu << 23,
	HIT_STUN = 1llu << 25,
	SHIELD_TOUCH = 1llu << 29,
	POWER_SHIELD = 1llu << 35,
	FOLLOWER = 1llu << 36,
	SLEEP = 1llu << 38,
	DEAD = 1llu << 38,
	OFF_SCREEN = 1llu << 39,
};
template<> constexpr bool is_bitfield<StateFlags> = true;


enum class LCancel : uint8_t {
	NONE = 0,
	SUCCESS = 1,
	FAILURE = 2,
};


enum class HurtboxState : uint8_t {
	VULNERABLE = 0,
	INVULNERABLE = 1,
	INTANGIBLE = 2,
};


// The game tracks two different "velocities" per character, self-induced and
// knockback. These are added to obtain an effective velocity, which may be
// further modified by other factors like obstacles.
struct Velocities {
	float selfInducedAirX;
	float selfInducedY;
	Velocity knockback;
	float selfInducedGroundX;

	friend bool operator==(const Velocities&, const Velocities&) = default;
};


// Pre-frame update data, required to reconstruct a replay. Information is
// collected right before controller inputs are used to figure out the
// character's next action.
struct Pre {
	// The number of the frame. Starts at -123. Frame 0 is when the timer starts
	// counting down
	int32_t frameNumber;

	// Port
	Port port;

	// True for Nana and false otherwise
	bool isFollower;

	// The random seed at this point
	uint32_t randomSeed;

	// Character's action state
	ActionState state;

	// Character's position
	Position position;

	// Direction item is facing, -1 = facing left, +1 = facing right
	float direction;

	// Processed analog joystick position
	Position joystick;

	// Processed analog c-stick position
	Position cstick;

	// Trigger state
	Triggers triggers;

	// Button state
	Buttons buttons;

	// added(1.2.0) Raw x analog controller input (for UCF)
	std::optional<uint8_t> rawAnalogX;

	// added(1.4.0) Current damage percent
	std::optional<float> damage;

	friend bool operator==(const Pre&, const Pre&) = default;
};


// Post-frame update data, for making decisions about game states (such as
// computing stats). Information is collected at the end of collision detection,
// which is the last consideration of the game engine.
struct Post {
	// The number of the frame. Starts at -123. Frame 0 is when the timer starts
	// counting down
	int32_t frameNumber;

	// Port
	Port port;

	// True for Nana and false otherwise
	bool isFollower;

	// In-game character (can only change for Zelda/Sheik). Check on first frame
	// to determine if Zelda started as Sheik
	InGameCharacter character;

	// Character's action state
	ActionState state;

	// Character's position
	Position position;

	// Direction item is facing, -1 = facing left, +1 = facing right
	float direction;

	// Current damage percent
	float damage;

	// Current size of shield (range: [0, 60])
	float shield;

	// ID of the last attack that hit a player. Attacks that "hit" reflectors,
	// counters, or absorbers do not modify this field. Link/Young Link's
	// shield behaves as an absorber. If a reflector is used and the reflected
	// projectile hits the enemy, the ID of the projectile is used and not the
	// ID of the reflector. This field corresponds to move staling. Any attack
	// with ID 1 does not participate in staling. This field is set back to 0 on
	// death
	Attack lastAttackLanded;

	// Combo count as defined by the game
	uint8_t comboCount;

	// The player that last hit this player
	std::optional<uint8_t> lastHitBy;

	// Number of stocks remaining
	uint8_t stocks;

	// added(0.2.0) Number of frames action state has been active. Can have a
	// fractional component for certain actions
	std::optional<float> stateAge;

	// added(2.0.0) State flags
	std::optional<StateFlags> stateFlags;

	// added(2.0.0) Can be used for different things. While in hitstun, contains
	// hitstun frames remaining
	std::optional<float> hitStunRemaining;

	// added(2.0.0) True if character is airborne
	std::optional<bool> airborne;

	// added(2.0.0) ID of ground character is standing on, if any
	std::optional<uint16_t> lastGroundId;

	// added(2.0.0) Jumps remaining
	std::optional<uint8_t> jumpsRemaining;

	// added(2.0.0) L-cancel status, if any
	std::optional<LCancel> lCancel;

	// added(2.1.0)
	std::optional<HurtboxState> hurtboxState;

	// added(3.5.0)
	std::optional<Velocities> velocities;

	// added(3.8.0) Hitlag remaining, 0 means "not in hitlag"
	std::optional<float> hitlagRemaining;

	// added(3.11.0) Indicates the animation the character is in.
	// For Wait:
	//   2 = Wait1
	//   3 = Wait2
	//   4 = Wait3
	std::optional<uint32_t> animationIndex;

	friend bool operator==(const Post&, const Post&) = default;
};


struct Frame {
	static constexpr int32_t kFirstFrameNumber = -123;

	// Frame data for a given port. Can include two characters' frame data (ICs).
	struct Player {
		// Frame data for a given character. Includes both pre-frame and post-frame data.
		struct Data {
			std::optional<Pre> pre;
			std::optional<Post> post;

			friend bool operator==(const Data&, const Data&) = default;
		};

		// Frame data for the controlled character
		Data leader;

		// Frame data for the follower (Nana), if any
		std::optional<Data> follower;

		friend bool operator==(const Player&, const Player&) = default;
	};

	// Start-of-frame data.
	struct Start {
		// The number of the frame. Starts at -123. Frame 0 is when the timer
		// starts counting down
		int32_t frameNumber;

		// The random seed at the start of the frame
		uint32_t randomSeed;

		// added(3.10.0) The scene frame counter. Starts at 0 when the game
		// starts. Continues to count frames even if the game is paused. Can
		// be used to determine if a pause happened and how many frames it
		// lasted by looking for jumps in this value from frame to frame
		std::optional<int32_t> sceneFrameCounter;

		friend bool operator==(const Start&, const Start&) = default;
	};

	// End-of-frame data.
	struct End {
		// The number of the frame. Starts at -123. Frame 0 is when the timer
		// starts counting down
		int32_t frameNumber;

		// added(3.7.0) For non-rollback, this should always equal the frame
		// number. For rollback, this indicates the index of a frame which is
		// guaranteed not to happen again (finalized). This is very useful when
		// reading a file in real-time to make sure you are processing
		// "finalized" information.
		std::optional<int32_t> latestFinalizedFrame;

		friend bool operator==(const End&, const End&) = default;
	};

	// The number of the frame. Starts at -123. Frame 0 is when the timer starts
	// counting down. Indexes should never skip values, but may decrease if
	// rollbacks are enabled.
	int32_t frameNumber;

	// Frame data for each port (port 1 is at index 0; empty ports will contain
	// std::nullopt)
	std::array<std::optional<Player>, 4> ports;

	// added(3.0.0) Active items(includes projectiles)
	std::vector<Item> items;

	// added(2.2.0) Start-of-frame data
	std::optional<Start> start;

	// added(2.2.0) End-of-frame data
	std::optional<End> end;

	friend bool operator==(const Frame&, const Frame&) = default;
};

}  // namespace cpp_slippi

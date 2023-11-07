#pragma once

namespace cpp_slippi {

struct Position {
	// Negative is left, positive is right
	float x;
	// Negative is down, positive is up
	float y;

	friend constexpr bool operator==(const Position&, const Position&) = default;
};


struct Velocity {
	// Negative is left, positive is right
	float x;
	// Negative is down, positive is up
	float y;

	friend constexpr bool operator==(const Velocity&, const Velocity&) = default;
};


struct Direction {
	static constexpr float LEFT = -1.0f;
	static constexpr float RIGHT = 1.0f;
};

}  // namespace cpp_slippi

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

}  // namespace cpp_slippi

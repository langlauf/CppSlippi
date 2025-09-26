#pragma once

#include <cstdint>


namespace cpp_slippi {

struct WhispyBlowDirection {
	enum class Direction : uint8_t {
		NONE = 0,
		LEFT = 1,
		RIGHT = 2,
	};

	// The number of the frame. Starts at -123. Frame 0 is when the timer starts counting down
	int32_t frameNumber;

	// Which direction Whispy is blowing (0 = None, 1 = Left, 2 = Right)
	Direction direction;
};

}  // namespace cpp_slippi
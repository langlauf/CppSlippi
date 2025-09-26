#pragma once

#include <cstdint>


namespace cpp_slippi {

struct FodPlatforms {
	enum class Side : uint8_t {
		RIGHT = 0,
		LEFT = 1,
	};

	// The number of the frame. Starts at -123. Frame 0 is when the timer starts counting down
	int32_t frameNumber;

	// Which platform has moved.
	Side platform;

	// The platform's new height
	float height;
};

}  // namespace cpp_slippi
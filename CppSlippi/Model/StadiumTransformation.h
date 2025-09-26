#pragma once

#include <cstdint>


namespace cpp_slippi {

struct StadiumTransformation {
	enum class Event : uint16_t {
		UNKNOWN1 = 1,  // The meaning of this event is not documented. Possibly loading?
		INITIALIZE = 2,
		ON_MONITOR = 3,
		RECEDING = 4,
		RISING = 5,
		FINALIZE = 6,
		FINISH = 0,
	};

	enum class Type : uint16_t {
		FIRE = 3,
		GRASS = 4,
		NORMAL = 5,
		ROCK = 6,
		WATER = 9,
	};

	// The number of the frame. Starts at -123. Frame 0 is when the timer starts counting down
	int32_t frameNumber;

	// The subevent for each transformation.
	Event transformationEvent;

	// The current or upcoming transformation.
	Type transformationType;
};

}  // namespace cpp_slippi
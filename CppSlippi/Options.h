#pragma once

#include <cstdint>


namespace cpp_slippi {

// Options to control the behavior of parsing.
struct Options {
	enum class Rollback : uint8_t {
		ALL, FIRST, LAST
	};

	class InterestedEvents {
		CPP_SLIPPI_DEFINE_BITFLAG_ENUM(InterestedEvents, uint16_t,
			NONE = 0x0,
			GAME_START = 0x1,
			FRAME_START = 0x2,
			FRAME_PRE = 0x4,
			FRAME_POST = 0x8,
			FRAME_ITEM = 0x10,
			FRAME_END = 0x20,
			GAME_END = 0x40,
			METADATA = 0x80,
			GECKO_CODES = 0x100,

			GAME_EVENTS = GAME_START | GAME_END,
			FRAME_EVENTS = FRAME_START | FRAME_PRE | FRAME_POST | FRAME_ITEM | FRAME_END,
			ALL = GAME_EVENTS | FRAME_EVENTS | METADATA | GECKO_CODES);
	};

	// Which frames to keep when rollback occurs.
	Rollback rollback = Rollback::LAST;

	// Allows processing of only the events that are interesting.
	InterestedEvents interestedEvents = InterestedEvents::ALL;

	// Convert fullwidth ASCII characters to halfwidth (regular) characters.
	bool fullwidthToHalfwidth = true;
};

} // namespace cpp_slippi

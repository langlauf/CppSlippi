#pragma once

#include <cstdint>

#include "CppSlippi/Utils/Enums.h"


namespace cpp_slippi {

// Options to control the behavior of parsing.
struct Options {
	enum class Rollback : uint8_t {
		ALL, FIRST, LAST
	};
	
	enum class ShiftJisErrorHandling : uint8_t {
		// Throw an exception if any error is encountered in a ShiftJIS string.
		// The exception will attempt to describe the nature of the error.
		THROW_EXCEPTIONS,
		// Attempt to gracefully handle errors in ShiftJIS strings. Invalid
		// characters will be mapped to U+FFFD (REPLACEMENT CHARACTER), other
		// errors will cause the string to be truncated.
		GRACEFUL,
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
			FOD_PLATFORMS = 0x200,
			WHISPY_BLOW_DIRECTION = 0x400,
			STADIUM_TRANSFORMATION = 0x800,

			GAME_EVENTS = GAME_START | GAME_END,
			FRAME_EVENTS = FRAME_START | FRAME_PRE | FRAME_POST | FRAME_ITEM | FRAME_END,
			STAGE_EVENTS = FOD_PLATFORMS | WHISPY_BLOW_DIRECTION | STADIUM_TRANSFORMATION,
			ALL = GAME_EVENTS | FRAME_EVENTS | STAGE_EVENTS | METADATA | GECKO_CODES);
	};

	// Which frames to keep when rollback occurs.
	Rollback rollback = Rollback::LAST;

	// Allows processing of only the events that are interesting.
	InterestedEvents interestedEvents = InterestedEvents::ALL;

	// Convert fullwidth ASCII characters to halfwidth (regular) characters.
	bool fullwidthToHalfwidth = true;

	ShiftJisErrorHandling shiftJisErrorHandling = ShiftJisErrorHandling::THROW_EXCEPTIONS;
};

} // namespace cpp_slippi

#pragma once

#include <cstdint>

#include "CppSlippi/Utils/BitFields.h"


namespace cpp_slippi {

class Buttons {
public:
	enum class Logical : uint32_t {
		NONE = 0,
		DPAD_LEFT = 1u << 0,
		DPAD_RIGHT = 1u << 1,
		DPAD_DOWN = 1u << 2,
		DPAD_UP = 1u << 3,
		Z = 1u << 4,
		R = 1u << 5,
		L = 1u << 6,
		A = 1u << 8,
		B = 1u << 9,
		X = 1u << 10,
		Y = 1u << 11,
		START = 1u << 12,
		JOYSTICK_UP = 1u << 16,
		JOYSTICK_DOWN = 1u << 17,
		JOYSTICK_LEFT = 1u << 18,
		JOYSTICK_RIGHT = 1u << 19,
		CSTICK_UP = 1u << 20,
		CSTICK_DOWN = 1u << 21,
		CSTICK_LEFT = 1u << 22,
		CSTICK_RIGHT = 1u << 23,
		TRIGGER_ANALOG = 1u << 31,
	};

	enum class Physical : uint16_t {
		NONE = 0,
		DPAD_LEFT = 1 << 0,
		DPAD_RIGHT = 1 << 1,
		DPAD_DOWN = 1 << 2,
		DPAD_UP = 1 << 3,
		Z = 1 << 4,
		R = 1 << 5,
		L = 1 << 6,
		A = 1 << 8,
		B = 1 << 9,
		X = 1 << 10,
		Y = 1 << 11,
		START = 1 << 12,
	};

	Logical logical;
	Physical physical;

	friend constexpr bool operator==(const Buttons&, const Buttons&) = default;
};

template<> constexpr bool is_bitfield<Buttons::Logical> = true;
template<> constexpr bool is_bitfield<Buttons::Physical> = true;


}  // namespace cpp_slippi

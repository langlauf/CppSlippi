#pragma once

#include <cstdint>
#include <string_view>

#include "CppSlippi/Utils/Enums.h"


namespace cpp_slippi {

class Port {
	CPP_SLIPPI_DEFINE_ENUM(Port, int8_t,
		NONE = -1,
		P1 = 0,
		P2 = 1,
		P3 = 2,
		P4 = 3);

public:
	static constexpr std::array<Port, 4> values() {
		return {P1, P2, P3, P4};
	}

	constexpr std::string_view toString() const {
		switch (value) {
			case P1:
				return "P1";
			case P2:
				return "P2";
			case P3:
				return "P3";
			case P4:
				return "P4";
			default:
				return "None";
		}
	}
};

}  // namespace cpp_slippi

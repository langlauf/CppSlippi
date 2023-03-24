#pragma once

#include <cstdint>


namespace cpp_slippi {

struct Triggers {
	struct Physical {
		float left;
		float right;

		friend constexpr bool operator==(const Physical&, const Physical&) = default;
	};

	float logical;
	Physical physical;

	friend constexpr bool operator==(const Triggers&, const Triggers&) = default;
};

}  // namespace cpp_slippi

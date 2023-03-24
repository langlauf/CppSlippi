#pragma once

#include <optional>


namespace cpp_slippi::parsing {

// Get the value from an optional or create a default value in place.
template<typename T>
T& getOrCreate(std::optional<T>& opt) {
	if (!opt) {
		opt.emplace();
	}
	return *opt;
}

}  // namespace cpp_slippi::parsing

#pragma once

#include <string>
#include <string_view>


namespace cpp_slippi::parsing {

class ShiftJisConverter {
public:
	explicit ShiftJisConverter(bool fullwidthToHalfwidth) : fullwidthToHalfwidth_(fullwidthToHalfwidth) {}

	// Convert the given Shfit-JIS string to UTF-8. If fullwidthToHalfwidth is true,
	// then any fullwidth ASCII characters will also be converted to halfwidth
	// (regular) characters.
	std::string toUtf8(std::string_view shiftJis) const;

private:
	bool fullwidthToHalfwidth_;
};

}  // namespace cpp_slippi::parsing

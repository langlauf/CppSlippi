#pragma once

#include <string>
#include <string_view>

#include "CppSlippi/Options.h"
#include "CppSlippi/Utils/Exceptions.h"


namespace cpp_slippi::parsing {

class ShiftJisConverter {
public:
	explicit ShiftJisConverter(bool fullwidthToHalfwidth, Options::ShiftJisErrorHandling errorHandling)
		: fullwidthToHalfwidth_(fullwidthToHalfwidth), errorHandling_(errorHandling) {}

	// Convert the given Shift-JIS string to UTF-8. If fullwidthToHalfwidth is
	// true, then any fullwidth ASCII characters will also be converted to
	// halfwidth (regular) characters.
	std::string toUtf8(std::string_view shiftJis) const;

private:
	bool fullwidthToHalfwidth_;
	Options::ShiftJisErrorHandling errorHandling_;	
};

// Indicates an error converting from ShiftJIS.
CPP_SLIPPI_DEFINE_EXCEPTION(ShiftJisException);

}  // namespace cpp_slippi::parsing

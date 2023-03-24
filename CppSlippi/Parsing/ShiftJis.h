#pragma once

#include <string>
#include <string_view>


namespace cpp_slippi::parsing {

// Convert the given Shfit-JIS string to UTF-8. If fullwidthToHalfwidth is true,
// then any fullwidth ASCII characters will also be converted to halfwidth
// (regular) characters.
std::string shiftJisToUtf8(std::string_view shiftJis, bool fullwidthToHalfwidth);

}  // namespace cpp_slippi::parsing

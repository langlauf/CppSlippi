#pragma once

#include <type_traits>


namespace cpp_slippi {

template<typename T>
constexpr std::underlying_type_t<T> underlying(T t) {
	return static_cast<std::underlying_type_t<T>>(t);
}

template<typename T> requires std::is_enum_v<T>
constexpr bool is_bitfield = false;

template<typename T>
concept bitfield = is_bitfield<T>;

template<bitfield T>
constexpr T operator|(T a, T b) {
	return static_cast<T>(underlying(a) | underlying(b));
}

template<bitfield T>
constexpr T operator&(T a, T b) {
	return static_cast<T>(underlying(a) & underlying(b));
}

template<bitfield T>
constexpr T operator^(T a, T b) {
	return static_cast<T>(underlying(a) ^ underlying(b));
}

template<bitfield T>
constexpr T operator~(T a) {
	return static_cast<T>(~underlying(a));
}

}  // namespace cpp_slippi

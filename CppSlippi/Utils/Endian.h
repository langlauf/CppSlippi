#pragma once

#include <algorithm>
#include <bit>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <ranges>
#include <type_traits>

#include "CppSlippi/Utils/Enums.h"


namespace cpp_slippi {
namespace details {

template<typename T>
concept readable_class = requires {
	{ T::kEndian } -> std::same_as<const std::endian&>;
};

}  // namespace details


// This is intentionally left uninitialized so that using types with undefined
// endianness will be an error.
template<typename T>
inline constexpr std::nullopt_t kEndian = std::nullopt;

template<std::integral T>
inline constexpr std::endian kEndian<T> = std::endian::big;

template<std::floating_point T>
inline constexpr std::endian kEndian<T> = std::endian::big;

template<typename T>
requires std::is_enum_v<T>
inline constexpr std::endian kEndian<T> = kEndian<std::underlying_type_t<T>>;

template<utils::special_enum T>
inline constexpr std::endian kEndian<T> = kEndian<typename T::Enum>;

template<typename T, size_t N>
requires (std::same_as<T, char> || std::same_as<T, uint8_t>)
inline constexpr std::endian kEndian<std::array<T, N>> = std::endian::little;

template<details::readable_class T>
inline constexpr std::endian kEndian<T> = T::kEndian;


template<typename T>
concept little_endian = kEndian<T> == std::endian::little;

template<typename T>
concept big_endian = kEndian<T> == std::endian::big;

template<typename T>
concept native_endian = (kEndian<T> == std::endian::native || sizeof(T) == sizeof(uint8_t));


// The creator of UBJSON must be a sadist, because it uses big-endian. So we
// have to swap the byte order.
template<typename T>
constexpr T toNativeEndian(T t) {
	if constexpr (native_endian<T>) {
		return t;
	} else {
		// I'd prefer for stylistic reasons not to use an else block here, but
		// MSVC complains about unreachable code if I don't.
#ifdef _MSC_VER
		// MSVC optimizations are not as good, so use intrinsics.
		if constexpr (sizeof(T) == sizeof(uint8_t)) {
			return t;
		} else if constexpr (sizeof(T) == sizeof(uint16_t)) {
			uint16_t n = _byteswap_ushort(reinterpret_cast<uint16_t&>(t));
			return reinterpret_cast<T&>(n);
		} else if constexpr (sizeof(T) == sizeof(uint32_t)) {
			uint32_t n = _byteswap_ulong(reinterpret_cast<uint32_t&>(t));
			return reinterpret_cast<T&>(n);
		} else if constexpr (sizeof(T) == sizeof(uint64_t)) {
			uint64_t n = _byteswap_uint64(reinterpret_cast<uint64_t&>(t));
			return reinterpret_cast<T&>(n);
		} else {
#endif
			union {
				T t;
				unsigned char bytes[sizeof(T)];
			} source, dest;

			source.t = t;

			std::ranges::copy(std::ranges::reverse_view(source.bytes), dest.bytes);
			return dest.t;
#ifdef _MSC_VER
		}
#endif
	}
}

}  // namespace cpp_slippi

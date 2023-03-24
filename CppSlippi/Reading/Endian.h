#pragma once

#include <bit>
#include <concepts>
#include <type_traits>

#include "CppSlippi/Model/Game.h"
#include "CppSlippi/Model/Frame.h"
#include "CppSlippi/Model/Enums/Item.h"
#include "CppSlippi/Utils/Enums.h"


namespace cpp_slippi::reading {

// This is a utility header to define the endianness of types when they are
// read from .slp files. This is used so that we know when bytes need to be
// swapped.

template<typename T>
constexpr std::endian kEndian = std::endian::native;

template<std::integral T>
constexpr std::endian kEndian<T> = std::endian::big;

template<std::floating_point T>
constexpr std::endian kEndian<T> = std::endian::big;

template<typename T>
requires std::is_enum_v<T>
constexpr std::endian kEndian<T> = kEndian<std::underlying_type_t<T>>;

template<special_enum T>
constexpr std::endian kEndian<T> = kEndian<typename T::Enum>;

template<typename T>
requires std::is_class_v<T>
constexpr std::endian kEndian<T> = std::endian::native;

// Bit flag enums should be treated as native endian.
template<> constexpr std::endian kEndian<Game::Bitfield> = std::endian::native;
template<> constexpr std::endian kEndian<StateFlags> = std::endian::native;
template<> constexpr std::endian kEndian<ItemSpawns> = std::endian::native;

}  // namespace cpp_slippi::parsing

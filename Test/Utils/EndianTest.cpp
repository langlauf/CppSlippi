#include "CppSlippi/Utils/Endian.h"

#include "CppSlippi/Model.h"


namespace cpp_slippi {

// Primitives.
static_assert(kEndian<bool> == std::endian::big);
static_assert(kEndian<uint8_t> == std::endian::big);
static_assert(kEndian<uint16_t> == std::endian::big);
static_assert(kEndian<uint32_t> == std::endian::big);
static_assert(kEndian<int8_t> == std::endian::big);
static_assert(kEndian<int16_t> == std::endian::big);
static_assert(kEndian<int32_t> == std::endian::big);
static_assert(kEndian<float> == std::endian::big);
static_assert(kEndian<double> == std::endian::big);

// Arrays.
static_assert(kEndian<std::array<char, 10>> == std::endian::little);
static_assert(kEndian<std::array<uint8_t, 10>> == std::endian::little);

// Structs.
struct TestUnreadable {};
struct TestReadable {
	static constexpr std::endian kEndian = std::endian::little;
};

static_assert(!details::allow_read<TestUnreadable>);
static_assert(details::allow_read<TestReadable>);
static_assert(kEndian<TestReadable> == std::endian::little);
static_assert(kEndian<Slippi::Version> == std::endian::little);

// Enums.
static_assert(kEndian<ItemType> == std::endian::big);
static_assert(kEndian<Game::Bitfield> == std::endian::little);
static_assert(kEndian<StateFlags> == std::endian::little);
static_assert(kEndian<ItemSpawns> == std::endian::little);


}  // namespace cpp_slippi

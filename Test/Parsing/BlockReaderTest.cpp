#include "CppSlippi/Parsing/BlockReader.h"

#include <string>

#include <gtest/gtest.h>

#include "CppSlippi/Parsing/ParseException.h"
#include "CppSlippi/Reading/FixedBufferReader.h"
#include "CppSlippi/Reading/Reader.h"


using namespace std::string_literals;  // Used for literals containing null characters.
using cpp_slippi::reading::FixedBufferReader;


namespace cpp_slippi::parsing {

TEST(BlockReaderTest, TestExpect) {
	std::string str = "Hello, world"s;
	FixedBufferReader bufReader(str.data(), str.size());

	BlockReader reader(bufReader);
	reader.expect(0x0, 'H');
	reader.expect(0x7, 'w');
}

TEST(BlockReaderTest, TestExpect_Mismatch_ShouldThrow) {
	std::string str = "Hello, world"s;
	FixedBufferReader bufReader(str.data(), str.size());

	BlockReader reader(bufReader);
	EXPECT_THROW(reader.expect(0x0, 'W'), ParseException);
}

TEST(BlockReaderTest, TestExpect_BeyondStream_ShouldThrow) {
	std::string str = "Hello"s;
	FixedBufferReader bufReader(str.data(), str.size());

	BlockReader reader(bufReader);
	EXPECT_THROW(reader.expect(0x10, 'W'), ParseException);
}

TEST(BlockReaderTest, TestRead_Byte) {
	std::string str = "\x10";
	FixedBufferReader bufReader(str.data(), str.size());

	BlockReader reader(bufReader);
	EXPECT_EQ(reader.read<uint8_t>(0x0), 0x10);
}

TEST(BlockReaderTest, TestRead_Short) {
	std::string str = "\x10\x20"s;
	FixedBufferReader bufReader(str.data(), str.size());

	BlockReader reader(bufReader);
	EXPECT_EQ(reader.read<uint16_t>(0x0), 0x1020);
}

TEST(BlockReaderTest, TestRead_Int) {
	std::string str = "\x10\x20\x30\x40"s;
	FixedBufferReader bufReader(str.data(), str.size());

	BlockReader reader(bufReader);
	EXPECT_EQ(reader.read<uint32_t>(0x0), 0x10203040);
}

TEST(BlockReaderTest, TestRead_SignedInt) {
	std::string str = "\xFF\xFF\xFF\xFE"s;
	FixedBufferReader bufReader(str.data(), str.size());

	BlockReader reader(bufReader);
	EXPECT_EQ(reader.read<int32_t>(0x0), -2);
}

TEST(BlockReaderTest, TestRead_Float) {
	std::string str = "\x3f\x80\x00\x00"s;
	FixedBufferReader bufReader(str.data(), str.size());

	BlockReader reader(bufReader);
	EXPECT_EQ(reader.read<float>(0x0), 1.0);
}

TEST(BlockReaderTest, TestRead_Double) {
	std::string str = "\x3f\xf0\x00\x00\x00\x00\x00\x00"s;
	FixedBufferReader bufReader(str.data(), str.size());

	BlockReader reader(bufReader);
	EXPECT_EQ(reader.read<double>(0x0), 1.0);
}

TEST(BlockReaderTest, TestRead_Enum) {
	enum class TestEnum : uint32_t {
		A = 0,
		B = 1,
		C = 2,
	};

	std::string str = "\x00\x00\x00\x02"s;
	FixedBufferReader bufReader(str.data(), str.size());

	BlockReader reader(bufReader);
	EXPECT_EQ(reader.read<TestEnum>(0x0), TestEnum::C);
}

TEST(BlockReaderTest, TestRead_NBytesInt) {
	std::string str = "\x10\x20"s;
	FixedBufferReader bufReader(str.data(), str.size());

	BlockReader reader(bufReader);
	auto expected = reader.read<uint32_t, 2>(0x0);
	EXPECT_EQ(expected, 0x10200000);
}

TEST(BlockReaderTest, TestRead_Consecutive) {
	std::string str = "\x10\x20\x30\x40\x50\x60\x70\x80"s;
	FixedBufferReader bufReader(str.data(), str.size());

	BlockReader reader(bufReader);
	EXPECT_EQ(reader.read<uint32_t>(0x0), 0x10203040);
	EXPECT_EQ(reader.read<uint32_t>(0x4), 0x50607080);
}

TEST(BlockReaderTest, TestRead_Skipping) {
	std::string str = "\x10\x20\x30\x40..\x50\x60\x70\x80"s;
	FixedBufferReader bufReader(str.data(), str.size());

	BlockReader reader(bufReader);
	EXPECT_EQ(reader.read<uint32_t>(0x0), 0x10203040);
	EXPECT_EQ(reader.read<uint32_t>(0x6), 0x50607080);
}

TEST(BlockReaderTest, TestReadSubBlock) {
	std::string str = "Block{\x10\x20\x30\x40}\x50\x60\x70\x80"s;
	FixedBufferReader bufReader(str.data(), str.size());

	BlockReader reader(bufReader);

	uint32_t actual;
	reader.readSubBlock(0x6, 0x4, [&actual](BlockReader& reader) {
		actual = reader.read<uint32_t>(0x0);
	});
	EXPECT_EQ(actual, 0x10203040);
	// Offsets are still correct after the block.
	EXPECT_EQ(reader.read<uint32_t>(0xB), 0x50607080);
}

TEST(BlockReaderTest, TestReadFor) {
	std::string str = "Block{\x10\x20\x30\x40}{\x50\x60\x70\x80}\x90\xA0\xB0\xC0"s;
	FixedBufferReader bufReader(str.data(), str.size());

	BlockReader reader(bufReader);

	uint32_t actual[2];
	reader.readFor(0x5, 2, 0x6, [&actual](BlockReader& reader, size_t i) {
		actual[i] = reader.read<uint32_t>(0x1);
	});
	EXPECT_EQ(actual[0], 0x10203040);
	EXPECT_EQ(actual[1], 0x50607080);
	// Offsets are still correct after the loop.
	EXPECT_EQ(reader.read<uint32_t>(0x11), 0x90A0B0C0);
}

}  // namespace cpp_slippi::parsing

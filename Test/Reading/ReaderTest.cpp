#define _CRT_SECURE_NO_WARNINGS

#include "CppSlippi/Reading/Reader.h"

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <exception>
#include <filesystem>
#include <format>
#include <fstream>
#include <string>
#include <string_view>
#include <type_traits>

#include <gtest/gtest.h>

#include "CppSlippi/Reading/BufferedReader.h"
#include "CppSlippi/Reading/FileReader.h"
#include "CppSlippi/Reading/FixedBufferReader.h"
#include "CppSlippi/Reading/IStreamReader.h"
#include "CppSlippi/Reading/ReadException.h"
#include "CppSlippi/Utils/Exceptions.h"


using namespace std::string_view_literals;

namespace cpp_slippi::reading {

CPP_SLIPPI_DEFINE_EXCEPTION(TestSetupException);


static constexpr std::string_view kText =
"String: Hello, world\n"
"Integer: \x10\x20\x30\x40\n"
"Signed integer: \xFF\xFF\xFF\xFE\n"
"Float: \x3f\x80\x00\x00\n"
"Double: \x3f\xf0\x00\x00\x00\x00\x00\x00\n"
"Enum: \x00\x00\x00\x02"sv;

static constexpr size_t kStrStart = kText.find("Hello, world");
static constexpr size_t kIntStart = kText.find("\x10\x20\x30\x40"sv);
// Do to an MSVC compiler bug, this does not compute the correct answer at compile time.
static constexpr size_t kSignedIntStart = 51; /*kText.find("\xFF\xFF\xFF\xFE"sv);*/
static constexpr size_t kFloatStart = kText.find("\x3f\x80\x00\x00"sv);
static constexpr size_t kDoubleStart = kText.find("\x3f\xf0\x00\x00\x00\x00\x00\x00"sv);
static constexpr size_t kEnumStart = kText.find("\x00\x00\x00\x02"sv);
static constexpr size_t kFileEnd = kText.size();


template<typename R>
concept IsReader = std::derived_from<R, Reader>;

template<IsReader R>
class ReaderFixture;

template<>
class ReaderFixture<IStreamReader<std::fstream>> {
public:
	ReaderFixture(std::filesystem::path path, bool seekable) :
		stream_(path, std::ios::in | std::ios::binary),
		reader_(stream_, seekable) {}

	IStreamReader<std::fstream>& reader() {
		return reader_;
	}

private:
	std::fstream stream_;
	IStreamReader<std::fstream> reader_;
};

template<>
class ReaderFixture<FileReader> {
public:
	ReaderFixture(std::filesystem::path path, bool seekable) :
		file_(open(path)),
		reader_(file_, 0, seekable) {}

	~ReaderFixture() {
		fclose(file_);
	}

	FileReader& reader() {
		return reader_;
	}

private:
	static FILE* open(std::filesystem::path path) {
		FILE* file = fopen(path.string().c_str(), "rb");
		if (!file) {
			throw TestSetupException(std::format("Could not open file {}.", path.string()));
		}
		return file;
	}

	FILE* file_;
	FileReader reader_;
};

template<IsReader R>
class ReaderFixture<BufferedReader<R>> {
public:
	ReaderFixture(std::filesystem::path path, bool seekable) :
		fixture_(path, seekable),
		reader_(std::move(fixture_.reader()), 16) {}

	BufferedReader<R>& reader() {
		return reader_;
	}

private:
	ReaderFixture<R> fixture_;
	BufferedReader<R> reader_;
};

template<>
class ReaderFixture<FixedBufferReader> {
public:
	ReaderFixture(std::filesystem::path path, bool seekable) :
		// Ignore the file and just read the data directly from memory.
		reader_(FixedBufferReader(kText.data(), kText.size())) {}

	FixedBufferReader& reader() {
		return reader_;
	}

private:
	std::string buffer_;
	FixedBufferReader reader_;
};


template<IsReader R>
class ReaderTest : public testing::Test {
public:
	ReaderTest() : tempFile_(getTempFile()) {}

protected:
	static std::filesystem::path getTempFile() {
		return std::filesystem::temp_directory_path() / "ReaderTestTemp.txt";
	}

	static void SetUpTestSuite() {
		std::fstream out(getTempFile().string(), std::ios::out | std::ios::binary);
		out.write(kText.data(), kText.size());
	}

	R& getReader(bool seekable = true) {
		fixture_.emplace(tempFile_, seekable);
		return fixture_->reader();
	}

private:
	std::filesystem::path tempFile_;
	std::optional<ReaderFixture<R>> fixture_;
};

using ReaderTypes = ::testing::Types<
	IStreamReader<std::fstream>,
	FileReader,
	FixedBufferReader,
	BufferedReader<IStreamReader<std::fstream>>,
	BufferedReader<FileReader>>;
TYPED_TEST_SUITE(ReaderTest, ReaderTypes);

TYPED_TEST(ReaderTest, ExpectBytes) {
	auto& reader = this->getReader();

	reader.seek(kStrStart, SEEK_SET);
	reader.expectBytes("Hello");
}

TYPED_TEST(ReaderTest, Expect_Mismatch_ShouldThrow) {
	auto& reader = this->getReader();

	reader.seek(kStrStart, SEEK_SET);
	EXPECT_THROW(reader.expectBytes("world"), ReadException);
}

TYPED_TEST(ReaderTest, ExpectBytes_BufferEnds_ShouldThrow) {
	auto& reader = this->getReader();

	reader.seek(kFileEnd - 2, SEEK_SET);
	EXPECT_THROW(reader.expectBytes("\x10\x20\x30\x40"), ReadException);
}

TYPED_TEST(ReaderTest, Read_BufferEnds_ShouldThrow) {
	auto& reader = this->getReader();

	reader.seek(kFileEnd - 2, SEEK_SET);
	EXPECT_THROW(reader.template read<uint32_t>(), ReadException);
}

TYPED_TEST(ReaderTest, Read_Byte) {
	auto& reader = this->getReader();

	reader.seek(kIntStart, SEEK_SET);
	EXPECT_EQ(reader.template read<uint8_t>(), 0x10);
}

TYPED_TEST(ReaderTest, Read_Short) {
	auto& reader = this->getReader();

	reader.seek(kIntStart, SEEK_SET);
	EXPECT_EQ(reader.template read<uint16_t>(), 0x1020);
}

TYPED_TEST(ReaderTest, Read_Int) {
	auto& reader = this->getReader();

	reader.seek(kIntStart, SEEK_SET);
	EXPECT_EQ(reader.template read<uint32_t>(), 0x10203040);
}

TYPED_TEST(ReaderTest, Read_SignedInt) {
	auto& reader = this->getReader();

	reader.seek(kSignedIntStart, SEEK_SET);
	EXPECT_EQ(reader.template read<int32_t>(), -2);
}

TYPED_TEST(ReaderTest, Read_Float) {
	auto& reader = this->getReader();

	reader.seek(kFloatStart, SEEK_SET);
	EXPECT_EQ(reader.template read<float>(), 1.0);
}

TYPED_TEST(ReaderTest, Read_Double) {
	auto& reader = this->getReader();

	reader.seek(kDoubleStart, SEEK_SET);
	EXPECT_EQ(reader.template read<double>(), 1.0);
}

TYPED_TEST(ReaderTest, Read_Enum) {
	enum class TestEnum : uint32_t {
		A = 0,
		B = 1,
		C = 2,
	};

	auto& reader = this->getReader();

	reader.seek(kEnumStart, SEEK_SET);
	EXPECT_EQ(reader.template read<TestEnum>(), TestEnum::C);
}

TYPED_TEST(ReaderTest, Read_NBytesInt) {
	auto& reader = this->getReader();
	reader.seek(kIntStart, SEEK_SET);

	uint32_t expected = reader.template read<uint32_t, 2>();
	EXPECT_EQ(expected, 0x10200000);
}

TYPED_TEST(ReaderTest, Seek_FromCurrent) {
	auto& reader = this->getReader();

	// Move the read head first.
	reader.expectBytes("String: ");

	reader.seek(7, SEEK_CUR);
	reader.expectBytes("world");
}

TYPED_TEST(ReaderTest, Seek_FromBeginning) {
	auto& reader = this->getReader();

	// Move the read head first.
	reader.expectBytes("String: Hello");

	reader.seek(8, SEEK_SET);
	reader.expectBytes("Hello");
}

TYPED_TEST(ReaderTest, Seek_FromEnd_ShouldThrow) {
	auto& reader = this->getReader();

	EXPECT_THROW(reader.seek(7, SEEK_END), ReadException);
}

TYPED_TEST(ReaderTest, Seek_Nonseekable_Forward) {
	auto& reader = this->getReader(false);

	// Move the read head first.
	reader.expectBytes("String: ");

	reader.seek(reader.position() + 7, SEEK_SET);
	reader.expectBytes("world");
}

TYPED_TEST(ReaderTest, Seek_Nonseekable_Backward_ShouldThrow) {
	// FixedBufferReader ignores seekable parameter, so it is allowed to seek
	// backwards.
	if constexpr (std::is_same_v<TypeParam, FixedBufferReader>) {
		return;
	}

	auto& reader = this->getReader(false);

	// Move the read head first.
	reader.expectBytes("String: ");

	EXPECT_THROW(reader.seek(0, SEEK_SET), ReadException);
}

TYPED_TEST(ReaderTest, TestFor) {
	auto& reader = this->getReader();

	std::string actual;
	for (char c : reader) {
		actual += c;
	}
	EXPECT_EQ(actual, kText);
}

TYPED_TEST(ReaderTest, Position) {
	auto& reader = this->getReader();

	EXPECT_EQ(reader.position(), 0);
	
	reader.template read<int>();
	EXPECT_EQ(reader.position(), 4);
	reader.expectBytes("ng:");
	EXPECT_EQ(reader.position(), 7);
	reader.seek(kStrStart, SEEK_SET);
	EXPECT_EQ(reader.position(), kStrStart);
}

TYPED_TEST(ReaderTest, Eof) {
	auto& reader = this->getReader();

	EXPECT_FALSE(reader.eof());
	reader.seek(kFileEnd, SEEK_SET);
	EXPECT_TRUE(reader.eof());
}

}  // namespace cpp_slippi::reading

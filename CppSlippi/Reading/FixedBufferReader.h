#pragma once

#include <cstddef>

#include "CppSlippi/Reading/Reader.h"
#include "CppSlippi/Utils/Defines.h"


namespace cpp_slippi::reading {

// A reader that uses a fixed in-memory buffer. This API is suitable for memory-
// mapped files, or for files that have been fully read into memory first. It is
// also useful for testing.
class CPP_SLIPPI_API FixedBufferReader final : public Reader {
public:
	FixedBufferReader(const char* buffer, size_t size) :
		Reader(true),
		buffer_(buffer),
		bufferEnd_(buffer_ + size),
		ptr_(buffer_) {}

	bool eof() const override;

protected:
	char peek() override;
	size_t doRead(char* dest, size_t count) override;
	void doSeek(size_t offset, int dir) override;
	void doSkip(size_t offset) override;

private:
	const char* const buffer_;
	const char* const bufferEnd_;

	const char* ptr_;
};

}  // namespace cpp_slippi::reading

#pragma once

#include <cstdio>

#include "CppSlippi/Reading/Reader.h"
#include "CppSlippi/Utils/Defines.h"


namespace cpp_slippi::reading {

// A reader that uses the C FILE* API. This is faster than C++ std::istreams
// from my testing. The file MUST have been opened in binary mode. It is
// strongly recommended to use this with BufferedReader for the best
// performance.
class CPP_SLIPPI_API FileReader final : public Reader {
public:
	// Default buffer is 0 because this is expected to be used with
	// BufferedReader in most cases, which provides it's own buffer.
	explicit FileReader(FILE* file, size_t bufferSize = 0, bool seekable = true) : Reader(seekable), file_(file) {
		setvbuf(file, nullptr, bufferSize > 0 ? _IOFBF : _IONBF, bufferSize);
	}

	bool eof() const override;

protected:
	char peek() override;
	size_t doRead(char* dest, size_t count) override;
	void doSeek(size_t offset, int dir = SEEK_CUR) override;
	void doSkip(size_t offset) override;

private:
	FILE* const file_;
};

}  // namespace cpp_slippi::reading

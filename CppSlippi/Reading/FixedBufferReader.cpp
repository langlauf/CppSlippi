#include "FixedBufferReader.h"

#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <utility>


namespace cpp_slippi::reading {

bool FixedBufferReader::eof() const {
	return ptr_ >= bufferEnd_;
}

char FixedBufferReader::peek() {
	return *ptr_;
}

size_t FixedBufferReader::doRead(char* dest, size_t count) {
	size_t available = bufferEnd_ - ptr_;
	size_t toRead = std::min(available, count);
	memcpy(dest, ptr_, toRead);
	ptr_ += toRead;
	return toRead;
}

void FixedBufferReader::doSeek(size_t offset, int dir) {
	ptr_ = (dir == SEEK_SET ? buffer_ : ptr_) + offset;
}

void FixedBufferReader::doSkip(size_t offset) {
	ptr_ += offset;
}


}  // namespace cpp_slippi::reading

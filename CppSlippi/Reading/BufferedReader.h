#pragma once

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <cstdio>
#include <vector>

#include "CppSlippi/Reading/Reader.h"


namespace cpp_slippi::reading {

template<typename R>
requires std::derived_from<R, Reader>
class BufferedReader final : public Reader {
public:
	explicit BufferedReader(R reader, size_t bufferSize = 32*4096) :
			Reader(reader.seekable()),
			reader_(std::move(reader)),
			buffer_(bufferSize),
			available_(0),
			ptr_(buffer_.data()),
			eof_(false) {}

	bool eof() const override {
		return available_ == 0 && reader_.eof();
	}

protected:
	char peek() override {
		if (available_ == 0) {
			refillBuffer();
			if (available_ == 0) {
				throw ReadException("No bytes left to read.");
			}
		}
		return *ptr_;
	}

	size_t doRead(char* dest, size_t count) override {
		char* outPtr = dest;
		while (count > available_) {
			memcpy(outPtr, ptr_, available_);
			outPtr += available_;
			count -= available_;
			// Refill will update ptr_ and available_.
			if (refillBuffer() != buffer_.size()) {
				break;
			}
		}

		size_t toRead = std::min(count, available_);
		memcpy(outPtr, ptr_, toRead);
		outPtr += toRead;
		available_ -= toRead;
		ptr_ += toRead;
		return outPtr - dest;
	}

	void doSeek(size_t offset, int dir = SEEK_CUR) override {
		if (dir == SEEK_CUR && offset <= available_) {
			available_ -= offset;
			ptr_ += offset;
		} else {
			// If SEEK_SET, target is offset. If SEEK_CUR, target is offset - available.
			static_cast<Reader&>(reader_).doSeek(offset - dir * available_, dir);
			available_ = 0;
			ptr_ = buffer_.data();
		}
		eof_ = false;
	}

	void doSkip(size_t offset) override {
		if (offset <= available_) {
			ptr_ += offset;
			available_ -= offset;
		} else {
			static_cast<Reader&>(reader_).doSkip(offset);
			available_ = 0;
			ptr_ = buffer_.data();
		}
	}

private:
	size_t refillBuffer() {
		// Static cast is needed to access doRead.
		size_t read = static_cast<Reader&>(reader_).doRead(buffer_.data(), buffer_.size());
		if (read != buffer_.size()) {
			eof_ = true;
		}
		ptr_ = buffer_.data();
		available_ = read;
		return read;
	}

	R reader_;
	std::vector<char> buffer_;

	size_t available_;
	char* ptr_;
	bool eof_;
};

}  // namespace cpp_slippi::reading

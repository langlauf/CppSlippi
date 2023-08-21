#pragma once

#include <concepts>
#include <ios>
#include <istream>
#include <memory>

#include "CppSlippi/Reading/Reader.h"


namespace cpp_slippi::reading {


// A reader that uses C++ istreams. This is slower than the C FILE* API from my
// testing. The stream MUST have been opened in binary mode. It is strongly
// recommended to use this with BufferedReader for the best performance.
template<typename S>
requires std::derived_from<S, std::istream>
class IStreamReader final : public Reader {
public:
	explicit IStreamReader(std::unique_ptr<S> stream, bool seekable = true) : Reader(seekable), stream_(std::move(stream)) {
		stream_->exceptions(std::ios::badbit);
	}

	bool eof() const override {
		return stream_->eof();
	}

protected:
	char peek() override {
		int c = stream_->peek();
		if (c == S::traits_type::eof()) {
			throw ReadException("No bytes left to read.");
		}
		return static_cast<char>(c);
	}

	size_t doRead(char* dest, size_t count) override {
		stream_->read(dest, count);
		return static_cast<size_t>(stream_->gcount());
	}

	void doSeek(size_t offset, int dir) override {
		stream_->clear();
		stream_->seekg(offset, dirToSeekDir(dir));
		// Ensure that the EOF bit is set.
		stream_->peek();
	}

	void doSkip(size_t offset) override {
		stream_->ignore(offset);
	}

private:
	static constexpr std::ios::seekdir dirToSeekDir(int dir) {
		switch (dir) {
			case SEEK_SET: return std::ios::beg;
			case SEEK_CUR: return std::ios::cur;
			case SEEK_END: return std::ios::end;
			default:
				throw ReadException(std::format("{} is not a valid seek direction.", dir));
		}
	}

	std::unique_ptr<S> stream_;
};

}  // namespace cpp_slippi::reading

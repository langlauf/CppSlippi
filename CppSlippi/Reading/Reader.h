#pragma once

#include <algorithm>
#include <concepts>
#include <cstdint>
#include <format>
#include <iterator>
#include <ranges>

#include "CppSlippi/Reading/ReadException.h"
#include "CppSlippi/Utils/Defines.h"
#include "CppSlippi/Utils/Endian.h"


namespace cpp_slippi::reading {

class CPP_SLIPPI_API Reader {
protected:
	struct Iterator {
		explicit Iterator(Reader* reader) : reader_(reader) {}
		virtual ~Iterator() = default;

		using value_type = char;
		// None of these should matter and are probably wrong.
		using difference_type = ptrdiff_t;
		using reference = char&;
		using iterator_category = std::input_iterator_tag;

		char operator*() {
			return reader_->peek();
		}

		Iterator& operator++() {
			reader_->seek(1, SEEK_CUR);
			return *this;
		}

		friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
			return lhs.reader_ == rhs.reader_ && lhs.reader_->eof();
		}

		Reader* reader_;
	};

	// Allow BufferedReader to call doRead.
	template<typename R>
	requires std::derived_from<R, Reader>
	friend class BufferedReader;

public:
	Reader(bool seekable) : seekable_(seekable), position_(0) {}
	virtual ~Reader() = default;

	// Moveable but not copyable.
	Reader(const Reader&) = delete;
	Reader(Reader&&) = default;
	
	// N parameter allows the option to read less than sizeof(T).
	template<typename T, size_t N = sizeof(T)>
	T read() {
		union {
			T t;
			char bytes[N];
		} u;
		// Zero-initialize T so it will not contain uninitialized memory.
		u.t = {};

		read(u.bytes, N);
		return toNativeEndian(u.t);
	}

	template<typename T>
	size_t read(T* dest, size_t count) {
		size_t n = sizeof(T) * count;
		size_t bytesRead = doRead(reinterpret_cast<char*>(dest), n);
		if (bytesRead != n) {
			throw ReadException(std::format("Expected to read {} bytes at position {}, but got {}.",
				n, position_, bytesRead));
		}
		position_ += bytesRead;
		return bytesRead;
	}

	template<size_t N>
	void expectBytes(const char(&expected)[N]) {
		char bytes[N];
		size_t startingPosition = position_;
		read(bytes, N-1);
		bytes[N-1] = 0;  // Add null terminator.
		if (!std::ranges::equal(bytes, expected)) {
			throw ReadException(std::format("Expected {} at position {}, but got: {}",
				expected, startingPosition, bytes));
		}
	}

	void seek(size_t offset, int dir = SEEK_CUR);

	bool seekable() const {
		return seekable_;
	}

	size_t position() const;
	Iterator begin();
	Iterator end();

	virtual bool eof() const = 0;

protected:
	virtual size_t doRead(char* dest, size_t count) = 0;
	virtual char peek() = 0;
	virtual void doSeek(size_t offset, int dir) = 0;
	// Read and discard bytes, used for skipping ahead in non-seekable files.
	virtual void doSkip(size_t offset) = 0;

private:
	const bool seekable_;
	size_t position_;
};

}  // namespace cpp_slippi::reading

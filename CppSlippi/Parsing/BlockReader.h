#pragma once

#include <concepts>
#include <format>
#include <type_traits>

#include "CppSlippi/Parsing/ParseException.h"
#include "CppSlippi/Reading/Reader.h"


namespace cpp_slippi::parsing {

// Read from a stream using offsets relative to the initial offset. This makes
// it easier to implement the .slp spec literally.
class BlockReader {
public:
	explicit BlockReader(reading::Reader& stream, size_t currentOffset = 0)
		: stream_(stream),
		  startOffset_(stream_.position() - currentOffset),
		  currentOffset_(currentOffset) {}

	template<typename T> requires std::integral<T> || std::is_enum_v<T>
	void expect(size_t offset, T expected) {
		T actual = read<T>(offset);
		if (actual != expected) {
			if constexpr (std::integral<T>) {
				throw ParseException(std::format("Expected {} at position {}, but got: {}",
					expected,
					startOffset_ + currentOffset_ - sizeof(T),
					actual));
			} else {
				throw ParseException(std::format("Expected {} at position {}, but got: {}",
					static_cast<std::underlying_type_t<T>>(expected),
					startOffset_ + currentOffset_ - sizeof(T),
					static_cast<std::underlying_type_t<T>>(actual)));
			}
		}
	}

	// N parameter allows the option to read less than sizeof(T).
	template<typename T, size_t N = sizeof(T)>
	T read(size_t offset) {
		setStreamOffset(offset);
		currentOffset_ += N;
		return stream_.read<T, N>();
	}

	template<typename T>
	void read(size_t offset, T* dest, size_t count) {
		setStreamOffset(offset);
		currentOffset_ += sizeof(T) * count;
		stream_.read(dest, count);
	}

	// Invokes the given callback `times` times. Each time, reads from the
	// callback will use offset positions relative to the given offset, plus
	// i*loopSize.
	template<std::invocable<BlockReader&, size_t> F>
	void readFor(size_t offset, size_t times, size_t loopSize, F callback) {
		currentOffset_ -= offset;
		for (size_t i = 0; i < times; ++i) {
			callback(*this, i);
			currentOffset_ -= loopSize;
		}
		// Restore offset relative to parent block.
		currentOffset_ += offset + times*loopSize;
	}

	// Invokes the given callback. All reads from that callback will use
	// offset positions relative to the given offset. This makes it easier to
	// read sub-blocks.
	template<std::invocable<BlockReader&> F>
	void readSubBlock(size_t offset, size_t blockSize, F callback) {
		currentOffset_ -= offset;
		callback(*this);
		// Restore offset relative to parent block.
		currentOffset_ += offset;
	}

private:
	// Seek until the stream position matches the given offset.
	void setStreamOffset(size_t offset) {
		if (static_cast<ptrdiff_t>(offset) != currentOffset_) {
			if (static_cast<ptrdiff_t>(offset) < currentOffset_) {
				throw ParseException("Cannot seek backwards.");
			}
			stream_.seek(offset - currentOffset_);
			currentOffset_ = offset;
		}
	}

	reading::Reader& stream_;
	// Position of the stream when the reader was created.
	size_t startOffset_;
	// Tracks the current position of the stream relative to where the stream
	// was when this reader or the current block was created. This is used to
	// ensure that the stream is reading from the correct position.
	ptrdiff_t currentOffset_;
};

}  // namespace cpp_slippi::parsing

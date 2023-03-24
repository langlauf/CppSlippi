#include "Reader.h"

#include <cstring>


namespace cpp_slippi::reading {

void Reader::seek(size_t offset, int dir) {
	if (dir == SEEK_END) {
		throw ReadException("Seeking from the end is not supported.");
	}
	
	// This is called in the hot loop, so a fun little micro-optimization:
	// SEEK_SET=0 and SEEK_CUR=1, so this adds offset to position if dir is
	// SEEK_CUR, otherwise sets position to offset.
	size_t newPosition = dir * position_ + offset;
	if (!seekable_) {
		if (newPosition < position_) {
			throw ReadException("Reader is not seekable, cannot seek backwards.");
		}
		doSkip(newPosition - position_);
	} else {
		doSeek(offset, dir);
	}
	position_ = newPosition;
}

size_t Reader::position() const {
	return position_;
}

Reader::Iterator Reader::begin() {
	return Iterator(this);
}

Reader::Iterator Reader::end() {
	return Iterator(this);
}

}  // namespace cpp_slippi::reading

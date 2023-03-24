#include "FileReader.h"

#include "CppSlippi/Reading/ReadException.h"


namespace cpp_slippi::reading {

bool FileReader::eof() const {
	return feof(file_);
}

char FileReader::peek() {
	int c = fgetc(file_);
	if (c == EOF) {
		throw ReadException("No bytes left to read.");
	}
	ungetc(c, file_);
	return static_cast<char>(c);
}

size_t FileReader::doRead(char* dest, size_t count) {
	size_t read = fread(dest, sizeof(char), count, file_);
	if (read != count && ferror(file_)) {
		throw ReadException("Error reading file.");
	}
	return read;
}

void FileReader::doSeek(size_t offset, int dir) {
	if (int err = fseek(file_, static_cast<long>(offset), dir)) {
		throw ReadException(std::format("Seek failed with error {}.", err));
	}
	// Attempting to get the next character will update the eof flag. If the
	// state is EOF, the ungetc has no effect.
	ungetc(fgetc(file_), file_);
}

void FileReader::doSkip(size_t offset) {
	// This is probably not terribly efficient, but hopefully this is only
	// ever used for live replays, when performance doesn't matter.
	for (size_t i = 0; i < offset; ++i) {
		fgetc(file_);
	}
}

}  // namespace cpp_slippi::reading

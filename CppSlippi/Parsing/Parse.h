#pragma once

#include <cstdint>
#include <utility>

#include "CppSlippi/Handlers.h"
#include "CppSlippi/Options.h"
#include "CppSlippi/Reading/Reader.h"


namespace cpp_slippi::parsing {


class Parser {
public:
	Parser(Options options, Handlers& handlers, reading::Reader& reader) :
		options_(std::move(options)),
		handlers_(handlers),
		reader_(reader) {}

	void parseGame();

private:
	Options options_;
	Handlers& handlers_;
	reading::Reader& reader_;
};

}  // namespace cpp_slippi::parsing

#pragma once

#include <utility>

#include "CppSlippi/Handlers.h"
#include "CppSlippi/Options.h"
#include "CppSlippi/Reading/Reader.h"


namespace cpp_slippi::parsing {


class Parser {
public:
	explicit Parser(Options options) : options_(std::move(options)) {}

	void parseGame(Handlers& handlers, reading::Reader& reader);

private:
	Options options_;
};

}  // namespace cpp_slippi::parsing

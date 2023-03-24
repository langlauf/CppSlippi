#pragma once

#include <cstdio>

#include "CppSlippi/Handlers.h"
#include "CppSlippi/Model.h"
#include "CppSlippi/Options.h"
#include "CppSlippi/Reading/Reader.h"
#include "CppSlippi/Utils/Defines.h"


namespace cpp_slippi {

// Parse a replay from an input stream using the given event handlers.
// IMPORTANT: If the stream is a file stream, it must be opened in binary mode.
CPP_SLIPPI_API void parse(reading::Reader& reader, Handlers& handlers, Options options = {});

// Parse a replay from an input stream, returning a Game object.
// IMPORTANT: If the stream is a file stream, it must be opened in binary mode.
CPP_SLIPPI_API Game game(reading::Reader& reader, Options options = {});

}  // namespace cpp_slippi

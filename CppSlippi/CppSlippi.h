#pragma once

#include <cstdio>
#include <filesystem>
#include <memory>
#include <string_view>

#include "CppSlippi/Handlers.h"
#include "CppSlippi/Model.h"
#include "CppSlippi/Options.h"
#include "CppSlippi/Reading/Reader.h"
#include "CppSlippi/Utils/Defines.h"


namespace cpp_slippi {

// These functions are provided in three forms. The basic form takes a Reader
// parameter which controls how the replay file is read. This provides the most
// control over reading. Additionally two convenience forms are provided taking
// a filename and a char buffer respectively. Best performance may be achieved
// by using a memory-mapped file with a FixedBufferReader or the char buffer
// form (which is a wrapper over FixedBufferReader).

// Parse a replay using the given event handlers.
CPP_SLIPPI_API void parse(reading::Reader& reader, Handlers& handlers, Options options = {});
CPP_SLIPPI_API void parse(std::filesystem::path, Handlers& handlers, Options options = {});
CPP_SLIPPI_API void parse(std::string_view buffer, Handlers& handlers, Options options = {});

// Parse a replay returning a Game object.
CPP_SLIPPI_API Game game(reading::Reader& reader, Options options = {});
CPP_SLIPPI_API Game game(std::filesystem::path filename, Options options = {});
CPP_SLIPPI_API Game game(std::string_view buffer, Options options = {});

}  // namespace cpp_slippi

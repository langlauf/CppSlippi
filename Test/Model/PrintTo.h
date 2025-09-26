#pragma once

#include <ostream>

#include "CppSlippi/Model.h"

namespace cpp_slippi {

// Suppress output of model structs so we don't get a massive (and useless) hex
// dump.
void PrintTo(const Ucf& ucf, std::ostream* os);
void PrintTo(const Team& team, std::ostream* os);
void PrintTo(const Player& player, std::ostream* os);
void PrintTo(const Game::Start& start, std::ostream* os);
void PrintTo(const Game::End& end, std::ostream* os);
void PrintTo(const Metadata::Player& player, std::ostream* os);
void PrintTo(const Metadata& player, std::ostream* os);
void PrintTo(const Pre& pre, std::ostream* os);
void PrintTo(const Post& Post, std::ostream* os);
void PrintTo(const Item& item, std::ostream* os);
void PrintTo(const FodPlatforms& item, std::ostream* os);
void PrintTo(const WhispyBlowDirection& item, std::ostream* os);
void PrintTo(const StadiumTransformation& item, std::ostream* os);
void PrintTo(const Frame::Player& player, std::ostream* os);
void PrintTo(const Frame::Player::Data& data, std::ostream* os);
void PrintTo(const Frame& frame, std::ostream* os);

}  // namespace cpp_slippi

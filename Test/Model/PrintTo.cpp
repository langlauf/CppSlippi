#include "PrintTo.h"

#include <ostream>

#include "CppSlippi/Model.h"


namespace cpp_slippi {

// Suppress output of model structs so we don't get a massive (and useless) hex
// dump.
void PrintTo(const Ucf& ucf, std::ostream* os) {
	*os << "Ucf";
}

void PrintTo(const Team& team, std::ostream* os) {
	*os << "Team";
}

void PrintTo(const Player& player, std::ostream* os) {
	*os << "Player";
}

void PrintTo(const Game::Start& start, std::ostream* os) {
	*os << "Game::Start";
}

void PrintTo(const Game::End& end, std::ostream* os) {
	*os << "Game::End";
}

void PrintTo(const Metadata::Player& player, std::ostream* os) {
	*os << "Metadata::Player";
}

void PrintTo(const Metadata& player, std::ostream* os) {
	*os << "Metadata";
}

void PrintTo(const Pre& pre, std::ostream* os) {
	*os << "Pre";
}

void PrintTo(const Post& Post, std::ostream* os) {
	*os << "Post";
}

void PrintTo(const Item& item, std::ostream* os) {
	*os << "Item";
}

void PrintTo(const FodPlatforms& item, std::ostream* os) {
	*os << "FodPlatforms";
}

void PrintTo(const WhispyBlowDirection& item, std::ostream* os) {
	*os << "WhispyBlowDirection";
}

void PrintTo(const StadiumTransformation& item, std::ostream* os) {
	*os << "StadiumTransformation";
}

void PrintTo(const Frame::Player& player, std::ostream* os) {
	*os << "Frame::Player";
}

void PrintTo(const Frame::Player::Data& data, std::ostream* os) {
	*os << "Frame::Player::Data";
}

void PrintTo(const Frame& frame, std::ostream* os) {
	*os << "Frame";
}

}  // cpp_slippi

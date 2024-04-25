#define _CRT_SECURE_NO_WARNINGS

#include "CppSlippi.h"

#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <format>
#include <string_view>
#include <system_error>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include "CppSlippi/Handlers.h"
#include "CppSlippi/Model.h"
#include "CppSlippi/Options.h"
#include "CppSlippi/Parsing/Parse.h"
#include "CppSlippi/Reading/BufferedReader.h"
#include "CppSlippi/Reading/FileReader.h"
#include "CppSlippi/Reading/FixedBufferReader.h"
#include "CppSlippi/Reading/Reader.h"


namespace cpp_slippi {
namespace {

template<typename Fn>
class Defer {
public:
	Defer(Fn fn) : fn(fn) {}
	~Defer() { fn(); }
private:
	Fn fn;
};

class GameHandlers : public Handlers {
public:
	explicit GameHandlers(Game& game, Options options)
		: game_(game), options_(options) {}

	void geckoCodes(GeckoCodes& codes) override {
		game_.geckoCodes = std::move(codes);
	}

	void gameStart(Game::Start& start) override {
		game_.start = std::move(start);
	}

	void gameEnd(Game::End& end) override {
		game_.end = std::move(end);
	}

	void allFrames(std::vector<Frame>& frames) override {
		game_.frames = std::move(frames);
	}

	void metadata(Metadata& metadata, nlohmann::json& rawMetadata) override {
		game_.metadata = std::move(metadata);
		game_.rawMetadata = std::move(rawMetadata);
	}

	void hintNumFrames(uint32_t numFrames) override {
		game_.frames.reserve(numFrames);
	}

private:
	Game& game_;
	Options options_;
};

}  // namespace


void parse(reading::Reader& reader, Handlers& handlers, Options options) {
	parsing::Parser parser(options);
	parser.parseGame(handlers, reader);
}

void parse(std::filesystem::path filename, Handlers& handlers, Options options) {
	FILE* file = fopen(filename.string().c_str(), "rb");
	if (!file) {
		throw std::system_error(errno, std::generic_category(), std::format("Failed to open file {}.", filename.string()));
	}
	Defer defer([&] { fclose(file); });

	reading::BufferedReader<reading::FileReader> reader{reading::FileReader{file}};
	parse(reader, handlers, options);
}

void parse(std::string_view buffer, Handlers& handlers, Options options) {
	reading::FixedBufferReader reader(buffer.data(), buffer.size());
	parse(reader, handlers, options);
}

Game game(reading::Reader& reader, Options options) {
	Game game;
	GameHandlers handlers(game, options);
	parse(reader, handlers, options);
	return game;
}

Game game(std::filesystem::path filename, Options options) {
	Game game;
	GameHandlers handlers(game, options);
	parse(filename, handlers, options);
	return game;
}

Game game(std::string_view buffer, Options options) {
	Game game;
	GameHandlers handlers(game, options);
	parse(buffer, handlers, options);
	return game;
}

}  // namespace cpp_slippi

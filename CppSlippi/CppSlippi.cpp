#include "CppSlippi.h"

#include <memory>

#include "CppSlippi/Parsing/Parse.h"


namespace cpp_slippi {

void parse(reading::Reader& reader, Handlers& handlers, Options options) {
	parsing::Parser parser(options);
	parser.parseGame(handlers, reader);
}

Game game(reading::Reader& reader, Options options) {
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

	Game game;
	GameHandlers handlers(game, options);
	parse(reader, handlers, options);
	return game;
}

}  // namespace cpp_slippi

#pragma once

#include <vector>

#include "CppSlippi/Model.h"


namespace cpp_slippi {

// Inherit from this class and override it's methods to implement an event
// handler. Events are passed as mutable reference so that callers may move from
// them, if they wish. However they are temporary objects so do not store
// references to them.
//
// Handlers will not be called if the caller does not indicate interest in the
// corresponding event in the options.
//
// Note that if you move from frame, pre, post, end, or item events then the
// corresponding frame may not be correct.
class Handlers {
public:
	virtual void geckoCodes(GeckoCodes& codes) {}
	virtual void gameStart(Game::Start& start) {}
	virtual void gameEnd(Game::End& end) {}
	virtual void frameStart(Frame::Start& start) {}
	virtual void framePre(Pre& pre) {}
	virtual void framePost(Post& post) {}
	virtual void frameEnd(Frame::End& end) {}
	virtual void frameItem(Item& item) {}
	virtual void frame(Frame& frame) {}
	virtual void allFrames(std::vector<Frame>& frames) {}
	virtual void metadata(Metadata& metadata, nlohmann::json& rawMetadata) {}
	virtual void finalize() {}

	// Provides the number of frames in the replay, not counting rollbacks. This
	// may be used to pre-allocate memory, for example. This handler will not be
	// called if the number of frames is unavailable.
	virtual void hintNumFrames(uint32_t numFrames) {}
};

}  // namespace cpp_slippi

#pragma once

#include <memory>
#include <optional>

#include "CppSlippi/Handlers.h"
#include "CppSlippi/Model.h"
#include "CppSlippi/Options.h"


namespace cpp_slippi::parsing {

// Frame events require special handling depending on the rollback strategy.
// This class handles this logic.
class FrameEventProcessor {
public:
	static std::unique_ptr<FrameEventProcessor> create(
		Options::Rollback rollback, Handlers& handlers, std::optional<uint32_t> hintNumFrames);

	virtual ~FrameEventProcessor() = default;

	virtual void processStart(Frame::Start&) = 0;
	virtual void processPre(Pre&) = 0;
	virtual void processPost(Post&) = 0;
	virtual void processItem(Item&) = 0;
	virtual void processEnd(Frame::End&) = 0;
	virtual void finish() = 0;

protected:
	explicit FrameEventProcessor(Handlers& handlers, std::optional<uint32_t> hintNumFrames)
		: handlers_(handlers) {
		if (hintNumFrames) {
			frames_.reserve(*hintNumFrames);
		}
	}

	void sendEvent(Frame::Start& start);
	void sendEvent(Pre& pre);
	void sendEvent(Post& post);
	void sendEvent(Frame::End& end);
	void sendEvent(Item& item);
	
	template<typename E>
	void maybeSendEvent(std::optional<E>& event) {
		if (event) {
			sendEvent(*event);
		}
	}

	void sendPlayerData(Frame::Player::Data& data);
	void sendAllFrameEvents(Frame& frame);
	void sendFrame(Frame& frame);
	void sendAllFrames();

	Handlers& handlers_;
	std::vector<Frame> frames_;
};

}  // namespace cpp_slippi::parsing

#include "FrameEventProcessor.h"

#include <algorithm>
#include <concepts>
#include <cstdint>
#include <format>
#include <unordered_set>

#include "CppSlippi/Parsing/Utils.h"
#include "CppSlippi/Parsing/ParseException.h"


namespace cpp_slippi::parsing {
namespace {

template<typename E>
concept Event = requires(E event) {
	{ event.frameNumber } -> std::same_as<int32_t&>;
};

// Except for rollbacks, frames are expected to be received in order. This
// function throws an exception if any frame is skipped.
void checkExpectedNextFrame(int32_t foundFrameNumber, int32_t expectedFrameNumber) {
	if (foundFrameNumber > expectedFrameNumber) {
		throw ParseException(std::format("Event for frame {} appeared before expected next frame {}.",
			foundFrameNumber, expectedFrameNumber));
	}
}

}  // namespace


// A class that knows how to add events to a frame and knows which events have
// been added.
class FrameBuilder {
public:
	explicit FrameBuilder(Frame* frame) : frame_(frame) {}

	FrameBuilder(Frame* frame, int32_t frameNumber) : frame_(frame) {
		frame_->frameNumber = frameNumber;
	}

	void addEvent(const Frame::Start& start) {
		frame_->start = start;
	}

	void addEvent(const Pre& pre) {
		Frame::Player& player = getOrCreate(frame_->ports[pre.port]);

		if (pre.isFollower) {
			getOrCreate(player.follower).pre = pre;
		} else {
			player.leader.pre = pre;
		}
	}

	void addEvent(const Post& post) {
		Frame::Player& player = getOrCreate(frame_->ports[post.port]);

		if (post.isFollower) {
			getOrCreate(player.follower).post = post;
		} else {
			player.leader.post = post;
		}
	}

	void addEvent(const Item& item) {
		frame_->items.push_back(item);
	}

	void addEvent(const Frame::End& end) {
		frame_->end = end;
	}

	bool hasEvent(const Frame::Start& start) const {
		return hasStart();
	}

	bool hasEvent(const Pre& pre) const {
		return hasPre(pre.port, pre.isFollower);
	}

	bool hasEvent(const Post& post) const {
		return hasPost(post.port, post.isFollower);
	}

	bool hasEvent(const Item& item) const {
		return hasItem(item.spawnId);
	}

	bool hasEvent(const Frame::End& end) const {
		return hasEnd();
	}

	// Check that a frame is valid, throwing an exception if it is not.
	void validateFrame() const {
		for (Port port : {Port::P1, Port::P2, Port::P3, Port::P4}) {
			if ((hasPre(port, true) && !hasPre(port, false))
				|| (hasPost(port, true) && !hasPost(port, false))) {
				throw ParseException(std::format("Port {} has follower but no leader for frame {}.",
					port.toString(), frame_->frameNumber));
			}
		}
	}

	const Frame& getFrame() const {
		return *frame_;
	}

	Frame& getFrame() {
		return *frame_;
	}

	void reset() {
		// Delete everything but the framenumber.
		*frame_ = { .frameNumber = frame_->frameNumber };
	}

private:
	bool hasStart() const {
		return getFrame().start.has_value();
	}

	bool hasPre(Port port, bool isFollower) const {
		const auto& maybePort = frame_->ports[port];
		if (!maybePort) {
			return false;
		} else if (!isFollower) {
			return maybePort->leader.pre.has_value();
		} else {
			return maybePort->follower && maybePort->follower->pre;
		}
	}

	bool hasPost(Port port, bool isFollower) const {
		const auto& maybePort = frame_->ports[port];
		if (!maybePort) {
			return false;
		} else if (!isFollower) {
			return maybePort->leader.post.has_value();
		} else {
			return maybePort->follower && maybePort->follower->post;
		}
	}

	bool hasItem(uint32_t spawnId) const {
		// There are relatively few items at a time, so a linear search is
		// faster than using a hash table.
		for (const Item& item : getFrame().items) {
			if (item.spawnId == spawnId) {
				return true;
			}
		}
		return false;
	}

	bool hasEnd() const {
		return getFrame().end.has_value();
	}

	Frame* frame_;
};


namespace {

// When a rollback occurs, this processor uses all versions of a frame.
class AllFramesProcessor : public FrameEventProcessor {
public:
	AllFramesProcessor(Handlers& handlers, std::optional<uint32_t> hintNumFrames)
		: FrameEventProcessor(handlers, hintNumFrames),
		  latestFrame_(Frame::kFirstFrameNumber - 1) {}

	void processStart(Frame::Start& start) override {
		processEvent(start);
		handlers_.frameStart(start);
	}

	void processPre(Pre& pre) override {
		processEvent(pre);
		handlers_.framePre(pre);
	}

	void processPost(Post& post) override {
		processEvent(post);
		handlers_.framePost(post);
	}

	void processItem(Item& item) override {
		processEvent(item);
		handlers_.frameItem(item);
	}

	void processEnd(Frame::End& end) override {
		processEvent(end);
		handlers_.frameEnd(end);
	}

	void finish() override {
		if (!frames_.empty()) {
			sendFrame(frames_.back());
		}
		sendAllFrames();
	}

private:
	template<Event E>
	void processEvent(const E& event) {
		std::optional<FrameBuilder> builder =
			frames_.empty() ? std::nullopt : std::optional(FrameBuilder(&frames_.back()));
		if (!builder || event.frameNumber != builder->getFrame().frameNumber || builder->hasEvent(event)) {
			// This is a new frame, send the current frame and reset.
			checkExpectedNextFrame(event.frameNumber, latestFrame_ + 1);
			if (builder) {
				sendFrame(builder->getFrame());
			}
			latestFrame_ = std::max(event.frameNumber, latestFrame_);
			builder = FrameBuilder(&frames_.emplace_back(), event.frameNumber);
		}
		builder->addEvent(event);
	}

	int32_t latestFrame_;
};


// When a rollback occurs, this processor uses the first version of the frame.
class FirstFrameProcessor : public FrameEventProcessor {
public:
	FirstFrameProcessor(Handlers& handlers, std::optional<uint32_t> hintNumFrames)
		: FrameEventProcessor(handlers, hintNumFrames),
		  currentFrameNumber_(Frame::kFirstFrameNumber - 1),
		  buildingFrame_(false) {}

	void processStart(Frame::Start& start) override {
		processEvent(start);
	}

	void processPre(Pre& pre) override {
		processEvent(pre);
	}

	void processPost(Post& post) override {
		processEvent(post);
	}

	void processItem(Item& item) override {
		processEvent(item);
	}

	void processEnd(Frame::End& end) override {
		processEvent(end);
	}

	void finish() override {
		if (buildingFrame_) {
			sendAllFrameEvents(frames_.back());
		}
		sendAllFrames();
	}

private:
	template<Event E>
	void processEvent(const E& event) {
		std::optional<FrameBuilder> builder =
			!buildingFrame_ ? std::nullopt : std::optional(FrameBuilder(&frames_.back()));
		if (event.frameNumber > currentFrameNumber_) {
			// A new frame. Send the current frame and reset.
			checkExpectedNextFrame(event.frameNumber, currentFrameNumber_ + 1);
			if (builder) {
				sendAllFrameEvents(builder->getFrame());
			}
			builder = FrameBuilder(&frames_.emplace_back(), ++currentFrameNumber_);
			builder->addEvent(event);
			buildingFrame_ = true;
		} else if (builder && event.frameNumber == currentFrameNumber_ && !builder->hasEvent(event)) {
			builder->addEvent(event);
		} else if (builder) {
			// This is a rollback frame, the current frame must be done, so send
			// it and reset. Then ignore the new event.
			sendAllFrameEvents(builder->getFrame());
			buildingFrame_ = false;
		} else {
			// The remaining case is a rollback frame without an active current
			// frame. Just ignore the event.
		}
	}

	int32_t currentFrameNumber_;
	bool buildingFrame_;
};


// When a rollback occurs, this processor uses the last version of the frame.
class LastFrameProcessor : public FrameEventProcessor {
public:
	LastFrameProcessor(Handlers& handlers, std::optional<uint32_t> hintNumFrames)
		: FrameEventProcessor(handlers, hintNumFrames), nextFrameToPush_(Frame::kFirstFrameNumber) {}

	void processStart(Frame::Start& start) override {
		processEvent(start);
	}

	void processPre(Pre& pre) override {
		processEvent(pre);
	}

	void processPost(Post& post) override {
		processEvent(post);
	}

	void processItem(Item& item) override {
		processEvent(item);
	}

	void processEnd(Frame::End& end) override {
		processEvent(end);

		if (end.latestFinalizedFrame) {
			int32_t newLatestFinalizedFrame = *end.latestFinalizedFrame;
			for (; nextFrameToPush_ <= newLatestFinalizedFrame; ++nextFrameToPush_) {
				sendAllFrameEvents(getFrame(nextFrameToPush_));
			}
		}
	}

	void finish() override {
		for (; nextFrameToPush_ - Frame::kFirstFrameNumber < static_cast<int32_t>(frames_.size()); ++nextFrameToPush_) {
			sendAllFrameEvents(getFrame(nextFrameToPush_));
		}
		sendAllFrames();
	}

private:
	size_t frameNumberToIndex(int32_t frameNumber) {
		return frameNumber - Frame::kFirstFrameNumber;
	}

	int32_t indexToFrameNumber(size_t index) {
		return static_cast<int32_t>(index) + Frame::kFirstFrameNumber;
	}

	Frame& getFrame(int32_t frameNumber) {
		return frames_[frameNumberToIndex(frameNumber)];
	}

	// Returns the frameBuilder for the given frame, adding a new frame to the
	// queue if necessary.
	FrameBuilder getFrameBuilder(int32_t frameNumber) {
		if (frameNumber < nextFrameToPush_) {
			throw ParseException(std::format("Event for frame {} found after that frame was finalized.", frameNumber));
		} else if (frameNumber >= nextFrameToPush_ && frameNumberToIndex(frameNumber) < frames_.size()) {
			return FrameBuilder(&getFrame(frameNumber));
		} else {
			checkExpectedNextFrame(frameNumber, indexToFrameNumber(frames_.size()));
			return FrameBuilder(&frames_.emplace_back(), frameNumber);
		}
	}

	template<typename E>
	void processEvent(const E& event) {
		FrameBuilder frameBuilder = getFrameBuilder(event.frameNumber);
		if (frameBuilder.hasEvent(event)) {
			// This is a new rollback frame, so discard what we had and reset.
			frameBuilder.reset();
		}
		frameBuilder.addEvent(event);
	}

	int32_t nextFrameToPush_;
};

}  // namespace


std::unique_ptr<FrameEventProcessor> FrameEventProcessor::create(
		Options::Rollback rollback, Handlers& handlers, std::optional<uint32_t> hintNumFrames) {
	switch (rollback) {
		case Options::Rollback::ALL:
			return std::make_unique<AllFramesProcessor>(handlers, hintNumFrames);
		case Options::Rollback::FIRST:
			return std::make_unique<FirstFrameProcessor>(handlers, hintNumFrames);
		default:
			return std::make_unique<LastFrameProcessor>(handlers, hintNumFrames);
	}
}

void FrameEventProcessor::sendEvent(Frame::Start& start) {
	handlers_.frameStart(start);
}

void FrameEventProcessor::sendEvent(Pre& pre) {
	handlers_.framePre(pre);
}

void FrameEventProcessor::sendEvent(Post& post) {
	handlers_.framePost(post);
}

void FrameEventProcessor::sendEvent(Frame::End& end) {
	handlers_.frameEnd(end);
}

void FrameEventProcessor::sendEvent(Item& item) {
	handlers_.frameItem(item);
}

void FrameEventProcessor::sendPlayerData(Frame::Player::Data& data) {
	maybeSendEvent(data.pre);
	maybeSendEvent(data.post);
}

void FrameEventProcessor::sendAllFrameEvents(Frame& frame) {
	maybeSendEvent(frame.start);
	for (auto& player : frame.ports) {
		if (player) {
			sendPlayerData(player->leader);

			if (player->follower) {
				sendPlayerData(*player->follower);
			}
		}
	}
	for (Item& item : frame.items) {
		sendEvent(item);
	}
	maybeSendEvent(frame.end);
	sendFrame(frame);
}

void FrameEventProcessor::sendFrame(Frame& frame) {
	FrameBuilder(&frame).validateFrame();
	handlers_.frame(frame);
};

void FrameEventProcessor::sendAllFrames() {
	handlers_.allFrames(frames_);
}

}  // namespace cpp_slippi::parsing

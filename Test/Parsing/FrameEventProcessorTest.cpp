#include "CppSlippi/Parsing/FrameEventProcessor.h"

#include <optional>
#include <memory>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "CppSlippi/Handlers.h"
#include "CppSlippi/Model.h"
#include "CppSlippi/Options.h"
#include "CppSlippi/Parsing/ParseException.h"
#include "CppSlippi/Parsing/Utils.h"
#include "Model/Matchers.h"


using testing::_;
using testing::InSequence;
using testing::Pointer;


namespace cpp_slippi::parsing {
namespace {

// Utility for creating testing frames.
class FrameBuilder {
public:
	explicit FrameBuilder(int32_t frameNumber) {
		frame_.frameNumber = frameNumber;
	}

	FrameBuilder& addStart() {
		frame_.start = Frame::Start {
			.frameNumber = frame_.frameNumber,
			.randomSeed = 80085,
		};
		return *this;
	}

	FrameBuilder& addPre(Port port, bool isFollower = false) {
		playerData(port, isFollower).pre = {
			.frameNumber = frame_.frameNumber,
			.port = port,
			.isFollower = isFollower,
			.position = {
				.x = 1.0,
				.y = 2.0,
			},
			.direction = -1.0,
			.buttons = {
				.logical = Buttons::Logical::A | Buttons::Logical::X | Buttons::Logical::JOYSTICK_DOWN,
				.physical = Buttons::Physical::A | Buttons::Physical::X,
			},
		};
		return *this;
	}

	FrameBuilder& addPost(Port port, bool isFollower = false) {
		playerData(port, isFollower).post = {
			.frameNumber = frame_.frameNumber,
			.port = port,
			.isFollower = isFollower,
			.position = {
				.x = 1.0,
				.y = 2.0,
			},
			.direction = -1.0,
			.shield = 1.0,
		};
		return *this;
	}

	FrameBuilder& addPlayer(Port port, bool isFollower = false) {
		return this
			->addPre(port, isFollower)
			.addPost(port, isFollower);
	}

	FrameBuilder& addItem() {
		frame_.items.push_back(Item {
			.frameNumber = frame_.frameNumber,
			.type = ItemType::BOB_OMB,
			.direction = 1.0,
			.velocity = {
				.x = 3.0,
				.y = 4.0,
			},
			.position = {
				.x = 1.0,
				.y = 2.0,
			},
			.spawnId = static_cast<uint32_t>(frame_.items.size()),
		});
		return *this;
	}

	FrameBuilder& addEnd(std::optional<int32_t> latestFinalizedFrame = std::nullopt) {
		frame_.end= Frame::End{
			.frameNumber = frame_.frameNumber,
			.latestFinalizedFrame = latestFinalizedFrame,
		};
		return *this;
	}

	Frame build() const {
		return frame_;
	}

private:
	Frame::Player::Data& playerData(Port port, bool isFollower) {
		Frame::Player& player = getOrCreate(frame_.ports[port]);
		return isFollower ?
			getOrCreate(player.follower) :
			player.leader;
	}

	Frame frame_;
};


// A set of frames for testing rollback behavior.
struct RollbackFrames {
	RollbackFrames() : 
		frame1(FrameBuilder(Frame::kFirstFrameNumber)
			.addPlayer(Port::P1)
			.build()),
		frame1Rollback(FrameBuilder(Frame::kFirstFrameNumber)
			.addPlayer(Port::P1)
			.addItem()
			.build()),
		frame2(FrameBuilder(Frame::kFirstFrameNumber + 1)
			.addPlayer(Port::P1)
			.build()),
		frame2Rollback(FrameBuilder(Frame::kFirstFrameNumber + 1)
			.addPlayer(Port::P1)
			.addItem()
			.build()),
		frame3(FrameBuilder(Frame::kFirstFrameNumber + 2)
			.addPlayer(Port::P1)
			.build()),
		frame3Rollback(FrameBuilder(Frame::kFirstFrameNumber + 2)
			.addPlayer(Port::P1)
			.addItem()
			.build()),
		frame4(FrameBuilder(Frame::kFirstFrameNumber + 3)
			.addPlayer(Port::P1)
			.build()),
		frame4Rollback(FrameBuilder(Frame::kFirstFrameNumber + 3)
			.addPlayer(Port::P1)
			.addItem()
			.build()) {}

	Frame frame1;
	Frame frame1Rollback;
	Frame frame2;
	Frame frame2Rollback;
	Frame frame3;
	Frame frame3Rollback;
	Frame frame4;
	Frame frame4Rollback;
};


class MockHandlers : public Handlers {
public:
	MOCK_METHOD(void, frameStart, (Frame::Start&), (override));
	MOCK_METHOD(void, framePre, (Pre&), (override));
	MOCK_METHOD(void, framePost, (Post&), (override));
	MOCK_METHOD(void, frameItem, (Item&), (override));
	MOCK_METHOD(void, frameEnd, (Frame::End&), (override));
	MOCK_METHOD(void, frame, (Frame&), (override));
	MOCK_METHOD(void, allFrames, (std::vector<Frame>&), (override));
};


class BaseFrameProcessorTest {
public:
	explicit BaseFrameProcessorTest(Options::Rollback rollback) :
		processor(FrameEventProcessor::create(rollback, handlers, std::nullopt)) {}

protected:
	// Send all events for the given frame to the processor.
	void sendFrameEvents(Frame& frame) {
		if (frame.start) {
			processor->processStart(*frame.start);
		}
		for (auto& player : frame.ports) {
			if (player) {
				processor->processPre(*player->leader.pre);
				processor->processPost(*player->leader.post);
			}
		}
		for (auto& player : frame.ports) {
			if (player && player->follower) {
				processor->processPre(*player->follower->pre);
				processor->processPost(*player->follower->post);
			}
		}
		for (auto& item : frame.items) {
			processor->processItem(item);
		}
		if (frame.end) {
			processor->processEnd(*frame.end);
		}
	}

	// Convenience overload so that we can send temporaries.
	void sendFrameEvents(Frame&& frame) {
		sendFrameEvents(frame);
	}

	MockHandlers handlers;
	std::unique_ptr<FrameEventProcessor> processor;
};


class FrameEventProcessorTest :
	public testing::TestWithParam<Options::Rollback>,
	protected BaseFrameProcessorTest {
public:
	FrameEventProcessorTest() : BaseFrameProcessorTest(GetParam()) {}
};

TEST_P(FrameEventProcessorTest, FollowerWithoutLeader_ThrowsParseException) {
	Frame frame = FrameBuilder(Frame::kFirstFrameNumber)
		.addPlayer(Port::P1, /* isFollower = */ true)
		.build();
	processor->processPre(*frame.ports[Port::P1]->follower->pre);
	processor->processPost(*frame.ports[Port::P1]->follower->post);
	EXPECT_THROW(processor->finish(), ParseException);
}

TEST_P(FrameEventProcessorTest, SkippingFirstFrame_ThrowsParseException) {
	EXPECT_THROW(
		sendFrameEvents(FrameBuilder(Frame::kFirstFrameNumber + 1)
			.addPlayer(Port::P1)
			.build()),
		ParseException);
}

TEST_P(FrameEventProcessorTest, SkippingFrame_ThrowsParseException) {
	sendFrameEvents(
		FrameBuilder(Frame::kFirstFrameNumber)
			.addPlayer(Port::P1)
			.build());

	EXPECT_THROW(
		sendFrameEvents(FrameBuilder(Frame::kFirstFrameNumber + 2)
			.addPlayer(Port::P1)
			.build()),
		ParseException);
}

TEST_P(FrameEventProcessorTest, ReturnsAllFrameEventsBeforeFrame) {
	Frame frame = FrameBuilder(Frame::kFirstFrameNumber)
		.addStart()
		.addPlayer(Port::P1)
		.addPlayer(Port::P2)
		.addPlayer(Port::P2, true)
		.addItem()
		.addEnd()
		.build();

	{
		InSequence seq;
		EXPECT_CALL(handlers, frameStart(FrameStartEq(*frame.start)));
		EXPECT_CALL(handlers, framePre(PreEq(*frame.ports[Port::P1]->leader.pre)));
		EXPECT_CALL(handlers, framePost(PostEq(*frame.ports[Port::P1]->leader.post)));
		EXPECT_CALL(handlers, framePre(PreEq(*frame.ports[Port::P2]->leader.pre)));
		EXPECT_CALL(handlers, framePost(PostEq(*frame.ports[Port::P2]->leader.post)));
		EXPECT_CALL(handlers, framePre(PreEq(*frame.ports[Port::P2]->follower->pre)));
		EXPECT_CALL(handlers, framePost(PostEq(*frame.ports[Port::P2]->follower->post)));
		EXPECT_CALL(handlers, frameItem(ItemEq(frame.items[0])));
		EXPECT_CALL(handlers, frameEnd(FrameEndEq(*frame.end)));
		EXPECT_CALL(handlers, frame(FrameEq(frame)));
		EXPECT_CALL(handlers, allFrames(testing::Pointwise(FrameEq(), {frame})));
	}

	sendFrameEvents(frame);
	processor->finish();
}

TEST_P(FrameEventProcessorTest, ReturnsOnlyIncludedFrameEvents) {
	Frame frame = FrameBuilder(Frame::kFirstFrameNumber)
		.addPlayer(Port::P1)
		.addItem()
		.build();

	EXPECT_CALL(handlers, frameStart(_)).Times(0);
	EXPECT_CALL(handlers, framePre(_));
	EXPECT_CALL(handlers, framePost(_));
	EXPECT_CALL(handlers, frameItem(_));
	EXPECT_CALL(handlers, frameEnd(_)).Times(0);
	EXPECT_CALL(handlers, frame(FrameEq(frame)));

	sendFrameEvents(frame);
	processor->finish();
}

TEST_P(FrameEventProcessorTest, NoFrames_ReturnsNothing) {
	EXPECT_CALL(handlers, frameStart(_)).Times(0);
	EXPECT_CALL(handlers, framePre(_)).Times(0);
	EXPECT_CALL(handlers, framePost(_)).Times(0);
	EXPECT_CALL(handlers, frameItem(_)).Times(0);
	EXPECT_CALL(handlers, frameEnd(_)).Times(0);
	EXPECT_CALL(handlers, frame(_)).Times(0);

	processor->finish();
}

INSTANTIATE_TEST_SUITE_P(FrameEventProcessor, FrameEventProcessorTest,
	testing::Values(Options::Rollback::ALL, Options::Rollback::FIRST, Options::Rollback::LAST),
	[](const testing::TestParamInfo<Options::Rollback>& info) -> std::string {
		return info.param == Options::Rollback::ALL ? "ALL" :
		       info.param == Options::Rollback::FIRST ? "FIRST" : "LAST";
	});


class AllFrameProcessorTest :
	public testing::Test,
	protected BaseFrameProcessorTest {
public:
	AllFrameProcessorTest() : BaseFrameProcessorTest(Options::Rollback::ALL) {}
};

TEST_F(AllFrameProcessorTest, ReturnsAllFrames) {
	RollbackFrames frames;

	EXPECT_CALL(handlers, frame(FrameEq(frames.frame1)));
	EXPECT_CALL(handlers, frame(FrameEq(frames.frame2)));
	EXPECT_CALL(handlers, frame(FrameEq(frames.frame3)));
	EXPECT_CALL(handlers, frame(FrameEq(frames.frame4)));
	EXPECT_CALL(handlers, frame(FrameEq(frames.frame1Rollback)));
	EXPECT_CALL(handlers, frame(FrameEq(frames.frame2Rollback)));
	EXPECT_CALL(handlers, frame(FrameEq(frames.frame3Rollback)));
	EXPECT_CALL(handlers, frame(FrameEq(frames.frame4Rollback)));

	sendFrameEvents(frames.frame1);
	sendFrameEvents(frames.frame1Rollback);
	sendFrameEvents(frames.frame2);
	sendFrameEvents(frames.frame2Rollback);
	sendFrameEvents(frames.frame3);
	sendFrameEvents(frames.frame4);
	sendFrameEvents(frames.frame3Rollback);
	sendFrameEvents(frames.frame4Rollback);
	processor->finish();
}


class FirstFrameProcessorTest :
	public testing::Test,
	protected BaseFrameProcessorTest {
public:
	FirstFrameProcessorTest() : BaseFrameProcessorTest(Options::Rollback::FIRST) {}
};

TEST_F(FirstFrameProcessorTest, ReturnsFirstFrame) {
	RollbackFrames frames;

	EXPECT_CALL(handlers, frame(FrameEq(frames.frame1)));
	EXPECT_CALL(handlers, frame(FrameEq(frames.frame2)));
	EXPECT_CALL(handlers, frame(FrameEq(frames.frame3)));
	EXPECT_CALL(handlers, frame(FrameEq(frames.frame4)));
	EXPECT_CALL(handlers, frame(FrameEq(frames.frame1Rollback)))
		.Times(0);
	EXPECT_CALL(handlers, frame(FrameEq(frames.frame2Rollback)))
		.Times(0);
	EXPECT_CALL(handlers, frame(FrameEq(frames.frame3Rollback)))
		.Times(0);
	EXPECT_CALL(handlers, frame(FrameEq(frames.frame4Rollback)))
		.Times(0);

	sendFrameEvents(frames.frame1);
	sendFrameEvents(frames.frame1Rollback);
	sendFrameEvents(frames.frame2);
	sendFrameEvents(frames.frame2Rollback);
	sendFrameEvents(frames.frame3);
	sendFrameEvents(frames.frame4);
	sendFrameEvents(frames.frame3Rollback);
	sendFrameEvents(frames.frame4Rollback);
	processor->finish();
}


class LastFrameProcessorTest :
	public testing::Test,
	protected BaseFrameProcessorTest {
public:
	LastFrameProcessorTest() : BaseFrameProcessorTest(Options::Rollback::LAST) {}
};

TEST_F(LastFrameProcessorTest, EventForFinalizedFrame_ThrowsParseException) {
	Frame frame = FrameBuilder(Frame::kFirstFrameNumber)
		.addStart()
		.addEnd(Frame::kFirstFrameNumber)
		.build();
	sendFrameEvents(frame);

	Frame::Start rollback = {
		.frameNumber = Frame::kFirstFrameNumber,
	};
	EXPECT_THROW(processor->processStart(rollback), ParseException);
}

TEST_F(LastFrameProcessorTest, ReturnsLastFrame) {
	RollbackFrames frames;

	EXPECT_CALL(handlers, frame(FrameEq(frames.frame1)))
		.Times(0);
	EXPECT_CALL(handlers, frame(FrameEq(frames.frame2)))
		.Times(0);
	EXPECT_CALL(handlers, frame(FrameEq(frames.frame3)))
		.Times(0);
	EXPECT_CALL(handlers, frame(FrameEq(frames.frame4)))
		.Times(0);
	EXPECT_CALL(handlers, frame(FrameEq(frames.frame1Rollback)));
	EXPECT_CALL(handlers, frame(FrameEq(frames.frame2Rollback)));
	EXPECT_CALL(handlers, frame(FrameEq(frames.frame3Rollback)));
	EXPECT_CALL(handlers, frame(FrameEq(frames.frame4Rollback)));

	sendFrameEvents(frames.frame1);
	sendFrameEvents(frames.frame1Rollback);
	sendFrameEvents(frames.frame2);
	sendFrameEvents(frames.frame2Rollback);
	sendFrameEvents(frames.frame3);
	sendFrameEvents(frames.frame4);
	sendFrameEvents(frames.frame3Rollback);
	sendFrameEvents(frames.frame4Rollback);
	processor->finish();
}

TEST_F(LastFrameProcessorTest, WithoutLastFinalizedFrame_ReturnsLastFrame) {
	Frame frame = FrameBuilder(Frame::kFirstFrameNumber)
		.addStart()
		.addEnd()
		.build();
	Frame rollbackFrame = FrameBuilder(Frame::kFirstFrameNumber)
		.addStart()
		.addItem()
		.addEnd()
		.build();

	EXPECT_CALL(handlers, frame(FrameEq(frame))).Times(0);
	EXPECT_CALL(handlers, frame(FrameEq(rollbackFrame)));

	sendFrameEvents(frame);
	sendFrameEvents(rollbackFrame);
	processor->finish();
}

}  // namespace
}  // namespace cpp_slippi::parsing

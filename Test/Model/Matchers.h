#pragma once

#include <string>
#include <tuple>
#include <type_traits>

#include <gmock/gmock.h>

#include "CppSlippi/Model.h"


namespace cpp_slippi {

template<typename T>
using TupleHelper = std::tuple<
	const typename std::remove_cvref_t<T>::value_type&,
	const typename std::remove_cvref_t<T>::value_type&>;

MATCHER_P2(MatchOptional, matcher, expected,
		expected ?
			std::string("actual and expected values ") + (negation ? "not" : "") +
				testing::DescribeMatcher<TupleHelper<arg_type>>(matcher, negation) :
			std::string("is ") + (negation ? "not" : "") + "empty") {
	if (!expected) {
		*result_listener << "which is " << (arg ? "" : "not") << " engaged";
		return !arg;
	}
	if (!arg) {
		*result_listener << "which is not engaged";
		return false;
	} else {
		testing::StringMatchResultListener value_listener;
		bool match = ExplainMatchResult(matcher, std::make_tuple(std::cref(*arg), std::cref(*expected)), &value_listener);
		*result_listener
			<< "whose value " << testing::PrintToString(*arg)
			<< (match ? " matches" : " doesn't match");
		if (!value_listener.str().empty()) {
			*result_listener << ", " << value_listener.str();
		}
		return match;
	}
}

#define MAKE_POINTWISE(name, pointwiseName, description)                     \
    MATCHER(pointwiseName, "are a pair that " + std::string(description)) {  \
        return ExplainMatchResult(                                           \
            name(std::get<0>(arg)),                                          \
            std::get<1>(arg),                                                \
            result_listener);                                                \
    }

#define MAKE_POINTWISE2(name, pointwiseName, description)                           \
    MATCHER_P(pointwiseName, matcher, "a pair that " + std::string(description)) {  \
        return ExplainMatchResult(                                                  \
            name(matcher, std::get<0>(arg)),                                        \
            std::get<1>(arg),                                                       \
            result_listener);                                                       \
    }

MAKE_POINTWISE(testing::StrEq, StrEq, negation ? "an equal pair" : "an unequal pair");
MAKE_POINTWISE2(testing::Pointwise, Pointwise, "pointwise match");
MAKE_POINTWISE2(MatchOptional, MatchOptional, "are equally empty or match");

// Defines both standard and pointwise matchers.
#define DEFINE_MATCHER(name, param, description)  \
    MAKE_POINTWISE(name, name, description);      \
    inline auto name(param)


DEFINE_MATCHER(MetadataPlayerEq, const Metadata::Player& expected, "equal MetadataPlayer") {
	return testing::AllOf(
		testing::Field("characters", &Metadata::Player::characters, testing::ContainerEq(expected.characters)),
		testing::Field("names", &Metadata::Player::names, testing::ContainerEq(expected.names)));
}

DEFINE_MATCHER(MetadataEq, const Metadata& expected, "equal Metadata") {
	return testing::AllOf(
		testing::Field("startAt", &Metadata::startAt, testing::Eq(expected.startAt)),
		testing::Field("lastFrame", &Metadata::lastFrame, testing::Eq(expected.lastFrame)),
		testing::Field("players", &Metadata::players, MatchOptional(
			Pointwise(
				MatchOptional(MetadataPlayerEq())),
			expected.players)),
		testing::Field("playedOn", &Metadata::playedOn, expected.playedOn));
}

DEFINE_MATCHER(SlippiEq, const Slippi& expected, "equal Slippi") {
	return testing::AllOf(
		testing::Field("version", &Slippi::version, testing::Eq(expected.version)));
}

DEFINE_MATCHER(UcfEq, const Ucf& expected, "equal Ucf") {
	return testing::AllOf(
		testing::Field("dashBack", &Ucf::dashBack, testing::Eq(expected.dashBack)),
		testing::Field("shieldDrop", &Ucf::shieldDrop, testing::Eq(expected.shieldDrop)));
}

DEFINE_MATCHER(TeamEq, const Team& expected, "equal Team") {
	return testing::AllOf(
		testing::Field("color", &Team::color, testing::Eq(expected.color)),
		testing::Field("shade", &Team::shade, testing::Eq(expected.shade)));
}

DEFINE_MATCHER(NetplayEq, const Netplay& expected, "equal Netplay") {
	return testing::AllOf(
		testing::Field("name", &Netplay::name, testing::StrEq(expected.name)),
		testing::Field("code", &Netplay::code, testing::StrEq(expected.code)),
		testing::Field("slippiUid", &Netplay::slippiUid, MatchOptional(StrEq(), expected.slippiUid)));
}

DEFINE_MATCHER(PlayerEq, const Player& expected, "equal Player") {
	return testing::AllOf(
		testing::Field("character", &Player::character, testing::Eq(expected.character)),
		testing::Field("type", &Player::type, testing::Eq(expected.type)),
		testing::Field("stocks", &Player::stocks, testing::Eq(expected.stocks)),
		testing::Field("costume", &Player::costume, testing::Eq(expected.costume)),
		testing::Field("team", &Player::team, MatchOptional(TeamEq(), expected.team)),
		testing::Field("handicap", &Player::handicap, testing::Eq(expected.handicap)),
		testing::Field("bitfield", &Player::bitfield, testing::Eq(expected.bitfield)),
		testing::Field("cpuLevel", &Player::cpuLevel, testing::Eq(expected.cpuLevel)),
		testing::Field("damageStart", &Player::damageStart, testing::Eq(expected.damageStart)),
		testing::Field("damageSpawn", &Player::damageSpawn, testing::Eq(expected.damageSpawn)),
		testing::Field("offenseRatio", &Player::offenseRatio, testing::Eq(expected.offenseRatio)),
		testing::Field("defenseRatio", &Player::defenseRatio, testing::Eq(expected.defenseRatio)),
		testing::Field("modelScale", &Player::modelScale, testing::Eq(expected.modelScale)),
		testing::Field("ucf", &Player::ucf, MatchOptional(UcfEq(), expected.ucf)),
		testing::Field("tag", &Player::tag, MatchOptional(StrEq(), expected.tag)),
		testing::Field("netplay", &Player::netplay, MatchOptional(NetplayEq(), expected.netplay)));
}

DEFINE_MATCHER(GameStartEq, const Game::Start& expected, "equal Game::Start") {
	return testing::AllOf(
		testing::Field("slippi", &Game::Start::slippi, SlippiEq(expected.slippi)),
		testing::Field("bitfield", &Game::Start::bitfield, testing::Eq(expected.bitfield)),
		testing::Field("timerBehavior", &Game::Start::timerBehavior, testing::Eq(expected.timerBehavior)),
		testing::Field("gameMode", &Game::Start::gameMode, testing::Eq(expected.gameMode)),
		testing::Field("isRainingBombs", &Game::Start::isRainingBombs, testing::Eq(expected.isRainingBombs)),
		testing::Field("isTeams", &Game::Start::isTeams, testing::Eq(expected.isTeams)),
		testing::Field("itemSpawnFrequency", &Game::Start::itemSpawnFrequency, testing::Eq(expected.itemSpawnFrequency)),
		testing::Field("selfDestructScore", &Game::Start::selfDestructScore, testing::Eq(expected.selfDestructScore)),
		testing::Field("stage", &Game::Start::stage, testing::Eq(expected.stage)),
		testing::Field("timer", &Game::Start::timer, testing::Eq(expected.timer)),
		testing::Field("itemSpawns", &Game::Start::itemSpawns, testing::Eq(expected.itemSpawns)),
		testing::Field("damageRatio", &Game::Start::damageRatio, testing::Eq(expected.damageRatio)),
		testing::Field("players", &Game::Start::players, testing::Pointwise(MatchOptional(PlayerEq()), expected.players)),
		testing::Field("randomSeed", &Game::Start::randomSeed, testing::Eq(expected.randomSeed)),
		testing::Field("isPal", &Game::Start::isPal, MatchOptional(testing::Eq(), expected.isPal)),
		testing::Field("scene", &Game::Start::scene, MatchOptional(testing::Eq(), expected.scene)),
		testing::Field("language", &Game::Start::language, MatchOptional(testing::Eq(), expected.language)),
		testing::Field("matchId", &Game::Start::matchId, MatchOptional(testing::Eq(), expected.matchId)),
		testing::Field("gameNumber", &Game::Start::gameNumber, MatchOptional(testing::Eq(), expected.gameNumber)),
		testing::Field("tiebreakerNumber", &Game::Start::tiebreakerNumber, MatchOptional(testing::Eq(), expected.gameNumber)));
}

DEFINE_MATCHER(GameEndEq, const Game::End& expected, "equal Game::End") {
	return testing::AllOf(
		testing::Field("method", &Game::End::method, testing::Eq(expected.method)),
		testing::Field("lrasInitiator", &Game::End::lrasInitiator, MatchOptional(testing::Eq(), expected.lrasInitiator)),
		testing::Field("playerPlacements", &Game::End::playerPlacements, MatchOptional(Pointwise(testing::Eq()), expected.playerPlacements)));
}

DEFINE_MATCHER(PositionEq, const Position& expected, "equal Position") {
	return testing::AllOf(
		testing::Field("x", &Position::x, testing::Eq(expected.x)),
		testing::Field("y", &Position::y, testing::Eq(expected.y)));
}

DEFINE_MATCHER(VelocityEq, const Velocity& expected, "equal Velocity") {
	return testing::AllOf(
		testing::Field("x", &Velocity::x, testing::Eq(expected.x)),
		testing::Field("y", &Velocity::y, testing::Eq(expected.y)));
}

DEFINE_MATCHER(VelocitiesEq, const Velocities& expected, "equal Velocities") {
	return testing::AllOf(
		testing::Field("selfInducedAirX", &Velocities::selfInducedAirX, testing::Eq(expected.selfInducedAirX)),
		testing::Field("selfInducedY", &Velocities::selfInducedY, testing::Eq(expected.selfInducedY)),
		testing::Field("knockback", &Velocities::knockback, VelocityEq(expected.knockback)),
		testing::Field("selfInducedGroundX", &Velocities::selfInducedGroundX, testing::Eq(expected.selfInducedGroundX)));
}

DEFINE_MATCHER(ButtonsEq, const Buttons& expected, "equal Buttons") {
	return testing::AllOf(
		testing::Field("logical", &Buttons::logical, testing::Eq(expected.logical)),
		testing::Field("physical", &Buttons::physical, testing::Eq(expected.physical)));
}

DEFINE_MATCHER(FrameStartEq, const Frame::Start& expected, "equal Frame::Start") {
	return testing::AllOf(
		testing::Field("frameNumber", &Frame::Start::frameNumber, testing::Eq(expected.frameNumber)),
		testing::Field("randomSeed", &Frame::Start::randomSeed, testing::Eq(expected.randomSeed)),
		testing::Field("sceneFrameCounter", &Frame::Start::sceneFrameCounter, MatchOptional(testing::Eq(), expected.sceneFrameCounter)));
}

DEFINE_MATCHER(PreEq, const Pre& expected, "equal Pre") {
	return testing::AllOf(
		testing::Field("frameNumber", &Pre::frameNumber, testing::Eq(expected.frameNumber)),
		testing::Field("port", &Pre::port, testing::Eq(expected.port)),
		testing::Field("isFollower", &Pre::isFollower, testing::Eq(expected.isFollower)),
		testing::Field("randomSeed", &Pre::randomSeed, testing::Eq(expected.randomSeed)),
		testing::Field("state", &Pre::state, testing::Eq(expected.state)),
		testing::Field("position", &Pre::position, PositionEq(expected.position)),
		testing::Field("direction", &Pre::direction, testing::Eq(expected.direction)),
		testing::Field("joystick", &Pre::joystick, PositionEq(expected.joystick)),
		testing::Field("cstick", &Pre::cstick, PositionEq(expected.cstick)),
		testing::Field("triggers", &Pre::triggers, testing::Eq(expected.triggers)),
		testing::Field("buttons", &Pre::buttons, testing::Eq(expected.buttons)),
		testing::Field("rawAnalogX", &Pre::rawAnalogX, MatchOptional(testing::Eq(), expected.rawAnalogX)),
		testing::Field("damage", &Pre::damage, MatchOptional(testing::Eq(), expected.damage)),
		testing::Field("rawAnalogY", &Pre::rawAnalogY, MatchOptional(testing::Eq(), expected.rawAnalogY)),
		testing::Field("rawCstickX", &Pre::rawCstickX, MatchOptional(testing::Eq(), expected.rawCstickX)),
		testing::Field("rawCstickY", &Pre::rawCstickY, MatchOptional(testing::Eq(), expected.rawCstickY)));
}

DEFINE_MATCHER(PostEq, const Post& expected, "equal Post") {
	return testing::AllOf(
		testing::Field("frameNumber", &Post::frameNumber, testing::Eq(expected.frameNumber)),
		testing::Field("port", &Post::port, testing::Eq(expected.port)),
		testing::Field("isFollower", &Post::isFollower, testing::Eq(expected.isFollower)),
		testing::Field("character", &Post::character, testing::Eq(expected.character)),
		testing::Field("state", &Post::state, testing::Eq(expected.state)),
		testing::Field("position", &Post::position, PositionEq(expected.position)),
		testing::Field("direction", &Post::direction, testing::Eq(expected.direction)),
		testing::Field("damage", &Post::damage, testing::Eq(expected.damage)),
		testing::Field("shield", &Post::shield, testing::Eq(expected.shield)),
		testing::Field("lastAttackLanded", &Post::lastAttackLanded, MatchOptional(testing::Eq(), expected.lastAttackLanded)),
		testing::Field("comboCount", &Post::comboCount, testing::Eq(expected.comboCount)),
		testing::Field("lastHitBy", &Post::lastHitBy, MatchOptional(testing::Eq(), expected.lastHitBy)),
		testing::Field("stocks", &Post::stocks, testing::Eq(expected.stocks)),
		testing::Field("stateAge", &Post::stateAge, MatchOptional(testing::Eq(), expected.stateAge)),
		testing::Field("flags", &Post::stateFlags, MatchOptional(testing::Eq(), expected.stateFlags)),
		testing::Field("hitStunRemaining", &Post::hitStunRemaining, MatchOptional(testing::Eq(), expected.hitStunRemaining)),
		testing::Field("airborne", &Post::airborne, MatchOptional(testing::Eq(), expected.airborne)),
		testing::Field("lastGroundId", &Post::lastGroundId, MatchOptional(testing::Eq(), expected.lastGroundId)),
		testing::Field("jumpsRemaining", &Post::jumpsRemaining, MatchOptional(testing::Eq(), expected.jumpsRemaining)),
		testing::Field("lCancel", &Post::lCancel, MatchOptional(testing::Eq(), expected.lCancel)),
		testing::Field("hurtboxState", &Post::hurtboxState, MatchOptional(testing::Eq(), expected.hurtboxState)),
		testing::Field("velocities", &Post::velocities, MatchOptional(VelocitiesEq(), expected.velocities)),
		testing::Field("hitlagRemaining", &Post::hitlagRemaining, MatchOptional(testing::Eq(), expected.hitlagRemaining)),
		testing::Field("animationIndex", &Post::animationIndex, MatchOptional(testing::Eq(), expected.animationIndex)));
}

DEFINE_MATCHER(FramePlayerDataEq, const Frame::Player::Data& expected, "equal Frame::Player::Data") {
	return testing::AllOf(
		testing::Field("pre", &Frame::Player::Data::pre, MatchOptional(PreEq(), expected.pre)),
		testing::Field("post", &Frame::Player::Data::post, MatchOptional(PostEq(), expected.post)));
}

DEFINE_MATCHER(FramePlayerEq, const Frame::Player& expected, "equal Frame::Player") {
	return testing::AllOf(
		testing::Field("leader", &Frame::Player::leader, FramePlayerDataEq(expected.leader)),
		testing::Field("follower", &Frame::Player::follower, MatchOptional(FramePlayerDataEq(), expected.follower)));
}

DEFINE_MATCHER(ItemEq, const Item& expected, "equal Item") {
	return testing::AllOf(
		testing::Field("frameNumber", &Item::frameNumber, testing::Eq(expected.frameNumber)),
		testing::Field("type", &Item::type, testing::Eq(expected.type)),
		testing::Field("state", &Item::state, testing::Eq(expected.state)),
		testing::Field("direction", &Item::direction, testing::Eq(expected.direction)),
		testing::Field("velocity", &Item::velocity, VelocityEq(expected.velocity)),
		testing::Field("position", &Item::position, PositionEq(expected.position)),
		testing::Field("damage", &Item::damage, testing::Eq(expected.damage)),
		testing::Field("expirationTimer", &Item::expirationTimer, testing::Eq(expected.expirationTimer)),
		testing::Field("spawnId", &Item::spawnId, testing::Eq(expected.spawnId)),
		testing::Field("misc1", &Item::misc1, testing::Eq(expected.misc1)),
		testing::Field("misc2", &Item::misc2, testing::Eq(expected.misc2)),
		testing::Field("misc3", &Item::misc3, testing::Eq(expected.misc3)),
		testing::Field("misc4", &Item::misc4, testing::Eq(expected.misc4)),
		testing::Field("owner", &Item::owner, MatchOptional(testing::Eq(), expected.owner)),
		testing::Field("instanceId", &Item::instanceId, MatchOptional(testing::Eq(), expected.instanceId)));
}

DEFINE_MATCHER(FodPlatformsEq, const FodPlatforms& expected, "equal FodPlatforms") {
	return testing::AllOf(
		testing::Field("frameNumber", &FodPlatforms::frameNumber, testing::Eq(expected.frameNumber)),
		testing::Field("platform", &FodPlatforms::platform, testing::Eq(expected.platform)),
		testing::Field("height", &FodPlatforms::height, testing::Eq(expected.height)));
}

DEFINE_MATCHER(WhispyBlowDirectionEq, const WhispyBlowDirection& expected, "equal WhispyBlowDirection") {
	return testing::AllOf(
		testing::Field("frameNumber", &WhispyBlowDirection::frameNumber, testing::Eq(expected.frameNumber)),
		testing::Field("direction", &WhispyBlowDirection::direction, testing::Eq(expected.direction)));
}

DEFINE_MATCHER(StadiumTransformationEq, const StadiumTransformation& expected, "equal StadiumTransformation") {
	return testing::AllOf(
		testing::Field("frameNumber", &StadiumTransformation::frameNumber, testing::Eq(expected.frameNumber)),
		testing::Field("transformationEvent", &StadiumTransformation::transformationEvent, testing::Eq(expected.transformationEvent)),
		testing::Field("transformationType", &StadiumTransformation::transformationType, testing::Eq(expected.transformationType)));
}

DEFINE_MATCHER(FrameEndEq, const Frame::End& expected, "equal Frame::End") {
	return testing::AllOf(
		testing::Field("frameNumber", &Frame::End::frameNumber, testing::Eq(expected.frameNumber)),
		testing::Field("latestFinalizedFrame", &Frame::End::latestFinalizedFrame, MatchOptional(testing::Eq(), expected.latestFinalizedFrame)));
}

DEFINE_MATCHER(FrameEq, const Frame& expected, "equal Frame") {
	return testing::AllOf(
		testing::Field("frameNumber", &Frame::frameNumber, testing::Eq(expected.frameNumber)),
		testing::Field("ports", &Frame::ports, testing::Pointwise(
			MatchOptional(FramePlayerEq()), expected.ports)),
		testing::Field("items", &Frame::items, testing::Pointwise(ItemEq(), expected.items)),
		testing::Field("start", &Frame::start, MatchOptional(FrameStartEq(), expected.start)),
		testing::Field("end", &Frame::end, MatchOptional(FrameEndEq(), expected.end)));
}

}  // cpp_slippi

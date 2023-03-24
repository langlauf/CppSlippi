#include "CppSlippi/Model/Enums/ActionState.h"

#include <gtest/gtest.h>

#include "CppSlippi/Model/Enums/Character.h"


namespace cpp_slippi {

// Ensure that a mapper exists for all characters, and that the mapper
// references the correct character.
TEST(ActionMapper, GetMapper_AllCharacters) {
	for (uint8_t i = InGameCharacter::MARIO; i <= InGameCharacter::SANDBAG; ++i) {
		InGameCharacter c = static_cast<InGameCharacter>(i);
		ASSERT_EQ(ActionMapper::get(c).character(), c);
	}
}

TEST(ActionMapper, GetMapper_InvalidCharacter_Throws) {
	ASSERT_THROW(ActionMapper::get(static_cast<InGameCharacter>(-1)), std::out_of_range);
}

TEST(ActionMapper, ToAction_Common) {
	auto& mapper = ActionMapper::get(InGameCharacter::KIRBY);
	ASSERT_EQ(mapper.toAction(ActionState::WALK_SLOW), Action::WALK_SLOW);
}

TEST(ActionMapper, ToAction_Character) {
	auto& mapper = ActionMapper::get(InGameCharacter::KIRBY);
	ASSERT_EQ(mapper.toAction(static_cast<ActionState>(353)), Action::SWALLOW_GROUND_STARTUP);
}

TEST(ActionMapper, ToAction_OutOfRange_ReturnsInvalid) {
	auto& mapper = ActionMapper::get(InGameCharacter::KIRBY);
	ASSERT_EQ(mapper.toAction(static_cast<ActionState>(999)), Action::UNKNOWN);
}

TEST(ActionMapper, ToActionState_Common) {
	auto& mapper = ActionMapper::get(InGameCharacter::KIRBY);
	ASSERT_EQ(mapper.toActionState(Action::WALK_SLOW), ActionState::WALK_SLOW);
}

TEST(ActionMapper, ToActionState_Character) {
	auto& mapper = ActionMapper::get(InGameCharacter::KIRBY);
	ASSERT_EQ(mapper.toActionState(Action::WALK_SLOW), ActionState::WALK_SLOW);
}

TEST(ActionMapper, ToActionState_OutOfRange_Throws) {
	auto& mapper = ActionMapper::get(InGameCharacter::KIRBY);
	ASSERT_THROW(mapper.toActionState(Action::UNKNOWN), NoKnownActionStateException);
	ASSERT_THROW(mapper.toActionState(static_cast<Action>(Action::kNumActions)), NoKnownActionStateException);
}

TEST(ActionMapper, ToActionState_NoActionStateForAction_Throws) {
	auto& mapper = ActionMapper::get(InGameCharacter::KIRBY);
	ASSERT_THROW(mapper.toActionState(Action::FALCON_PUNCH_GROUND), NoKnownActionStateException);
}


}  // namespace cpp_slippi

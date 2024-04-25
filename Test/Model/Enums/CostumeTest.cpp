#include "CppSlippi/Model/Enums/Costume.h"

#include <cstdint>
#include <stdexcept>

#include <gtest/gtest.h>

#include "CppSlippi/Model/Enums/Character.h"


namespace cpp_slippi {

// Ensure that a mapper exists for all characters, and that the mapper
// references the correct character.
TEST(CostumeMapper, GetMapper_AllCharacters) {
	for (uint8_t i = CssCharacter::CAPTAIN_FALCON; i <= CssCharacter::POPO; ++i) {
		CssCharacter c = static_cast<CssCharacter>(i);
		ASSERT_EQ(CostumeMapper::get(c).character(), c);
	}
}

TEST(CostumeMapper, GetMapper_InvalidCharacter_Throws) {
	ASSERT_THROW(CostumeMapper::get(static_cast<CssCharacter>(-1)), std::out_of_range);
}

TEST(CostumeMapper, NumCostumes) {
	auto& mapper = CostumeMapper::get(CssCharacter::KIRBY);
	ASSERT_EQ(mapper.numCostumes(), 6);
}

TEST(CostumeMapper, ToColor) {
	auto& mapper = CostumeMapper::get(CssCharacter::KIRBY);
	ASSERT_EQ(mapper.toColor(static_cast<Costume>(2)), Color::BLUE);
}

TEST(CostumeMapper, ToColor_OutOfRange_ReturnsInvalid) {
	auto& mapper = CostumeMapper::get(CssCharacter::KIRBY);
	ASSERT_EQ(mapper.toColor(static_cast<Costume>(7)), Color::INVALID);
}

TEST(CostumeMapper, ToCostume) {
	auto& mapper = CostumeMapper::get(CssCharacter::KIRBY);
	ASSERT_EQ(mapper.toCostume(Color::YELLOW), static_cast<Costume>(1));
}

TEST(CostumeMapper, ToCostume_OutOfRange_Throws) {
	auto& mapper = CostumeMapper::get(CssCharacter::KIRBY);
	ASSERT_THROW(mapper.toCostume(Color::INVALID), NoSuchColorException);
	ASSERT_THROW(mapper.toCostume(static_cast<Color>(Color::kNumColors)), NoSuchColorException);
}

TEST(CostumeMapper, ToCostume_NoCostumeWithColor_Throws) {
	auto& mapper = CostumeMapper::get(CssCharacter::KIRBY);
	ASSERT_THROW(mapper.toCostume(Color::BLACK), NoSuchColorException);
}

}  // namespace cpp_slippi

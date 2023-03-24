#pragma once

#include <array>
#include <cstdint>
#include <initializer_list>
#include <string_view>

#include "CppSlippi/Model/Enums/Character.h"
#include "CppSlippi/Utils/Enums.h"
#include "CppSlippi/Utils/Exceptions.h"


namespace cpp_slippi {

class Color;

// Definition not provided because costume colors indices are different for
// every character.
class Costume {
	DEFINE_ENUM(Costume, uint8_t);

public:
	// The maximum number of costumes that a single character may have.
	static constexpr uint8_t kMaxNumCostumes = 6;
};

class Color {
	DEFINE_ENUM(Color, int8_t,
		INVALID = -1,
		RED,
		GREEN,
		BLUE,
		WHITE,
		BLACK,
		BROWN,
		INDIGO,
		PINK,
		YELLOW,
		PURPLE,
		ORANGE,
		CYAN,
		NAVY);

public:
	static constexpr uint8_t kNumColors = NAVY + 1;

	constexpr std::string_view toString() const {
		switch (value) {
			case RED:
				return "red";
			case GREEN:
				return "green";
			case BLUE:
				return "blue";
			case WHITE:
				return "white";
			case BLACK:
				return "black";
			case BROWN:
				return "brown";
			case INDIGO:
				return "indigo";
			case PINK:
				return "pink";
			case YELLOW:
				return "yellow";
			case PURPLE:
				return "purple";
			case ORANGE:
				return "orange";
			case CYAN:
				return "cyan";
			case NAVY:
				return "navy";
			default:
				return "invalid color";
		}
	}
};


// Helper class for mapping between costumes and colors. Each instance of this
// class represents one CssCharacter.
class CostumeMapper {
public:
	CostumeMapper(const CostumeMapper&) = delete;
	CostumeMapper(CostumeMapper&&) = delete;

	// Get the mapper for the given character.
	static const CostumeMapper& get(CssCharacter character);

	CssCharacter character() const;

	// Get the correspoding color for this character and the given costume ID.
	// Will return Color::INVALID if the costume ID does not correspond to a
	// color for this character (costume >= numCostumes).
	Color toColor(Costume costume) const;

	// Get the costume ID for this character and the given color. Throws
	// NoSuchColorException if the given color does not exist for this
	// character.
	Costume toCostume(Color color) const;

	// Return the number of costumes available for this character.
	size_t numCostumes() const;

private:
	constexpr CostumeMapper(CssCharacter character, std::initializer_list<Color> colors);

	CssCharacter character_;
	std::array<Color, Costume::kMaxNumCostumes> toColor_;
	std::array<Costume, Color::kNumColors> toCostume_;
	size_t numCostumes_;
};


// Indicates that a requested color does not exist for a given character.
DEFINE_EXCEPTION(NoSuchColorException);

}  // namespace cpp_slippi
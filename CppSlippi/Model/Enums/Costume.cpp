#include "Costume.h"

#include <format>
#include <utility>

namespace cpp_slippi {
namespace {

constexpr std::array<Color, Costume::kMaxNumCostumes> buildToColor(std::initializer_list<Color> colors) {
	std::array<Color, Costume::kMaxNumCostumes> map;
	map.fill(Color::INVALID);
	int i = 0;
	for (Color c : colors) {
		map[i++] = c;
	}
	return map;
}

constexpr std::array<Costume, Color::kNumColors> buildToCostume(std::initializer_list<Color> colors) {
	std::array<Costume, Color::kNumColors> map;
	map.fill(static_cast<Costume>(-1));
	int i = 0;
	for (Color c : colors) {
		map[static_cast<size_t>(c)] = static_cast<Costume>(i++);
	}
	return map;
}

}  // namespace


constexpr CostumeMapper::CostumeMapper(CssCharacter character, std::initializer_list<Color> colors) :
	character_(character),
	toColor_(buildToColor(colors)),
	toCostume_(buildToCostume(colors)),
	numCostumes_(colors.size()) {}


const CostumeMapper& CostumeMapper::get(CssCharacter character) {
	/////////////////////////////////////////////
	// Character color definitions begin here. //
	/////////////////////////////////////////////
	static constexpr std::array kMappers = {
		CostumeMapper(CssCharacter::CAPTAIN_FALCON, {Color::INDIGO, Color::BLACK, Color::RED, Color::WHITE, Color::GREEN, Color::BLUE}),
		CostumeMapper(CssCharacter::DONKEY_KONG, {Color::WHITE, Color::RED, Color::BLUE, Color::GREEN}),
		CostumeMapper(CssCharacter::FOX, {Color::WHITE, Color::RED, Color::BLUE, Color::GREEN}),
		CostumeMapper(CssCharacter::GAME_AND_WATCH, {Color::BLACK, Color::RED, Color::BLUE, Color::GREEN}),
		CostumeMapper(CssCharacter::KIRBY, {Color::PINK, Color::YELLOW, Color::BLUE, Color::RED, Color::GREEN, Color::WHITE}),
		CostumeMapper(CssCharacter::BOWSER, {Color::GREEN, Color::RED, Color::BLUE, Color::BLACK}),
		CostumeMapper(CssCharacter::LINK, {Color::GREEN, Color::RED, Color::BLUE, Color::BLACK, Color::WHITE}),
		CostumeMapper(CssCharacter::LUIGI, {Color::GREEN, Color::WHITE, Color::BLUE, Color::RED}),
		CostumeMapper(CssCharacter::MARIO, {Color::RED, Color::YELLOW, Color::BLACK, Color::BLUE, Color::GREEN}),
		CostumeMapper(CssCharacter::MARTH, {Color::BLUE, Color::RED, Color::GREEN, Color::BLACK, Color::WHITE}),
		CostumeMapper(CssCharacter::MEWTWO, {Color::WHITE, Color::RED, Color::BLUE, Color::GREEN}),
		CostumeMapper(CssCharacter::NESS, {Color::RED, Color::YELLOW, Color::BLUE, Color::GREEN}),
		CostumeMapper(CssCharacter::PEACH, {Color::RED, Color::YELLOW, Color::WHITE, Color::BLUE, Color::GREEN}),
		CostumeMapper(CssCharacter::PIKACHU, {Color::YELLOW, Color::RED, Color::BLUE, Color::GREEN}),
		CostumeMapper(CssCharacter::ICE_CLIMBERS, {Color::BLUE, Color::GREEN, Color::ORANGE, Color::RED}),
		CostumeMapper(CssCharacter::JIGGLYPUFF, {Color::PINK, Color::RED, Color::BLUE, Color::GREEN, Color::YELLOW}),
		CostumeMapper(CssCharacter::SAMUS, {Color::RED, Color::PINK, Color::BLACK, Color::GREEN, Color::BLUE}),
		CostumeMapper(CssCharacter::YOSHI, {Color::GREEN, Color::RED, Color::BLUE, Color::YELLOW, Color::PINK, Color::CYAN}),
		CostumeMapper(CssCharacter::ZELDA, {Color::PINK, Color::RED, Color::BLUE, Color::GREEN, Color::WHITE}),
		CostumeMapper(CssCharacter::SHEIK, {Color::NAVY, Color::RED, Color::BLUE, Color::GREEN, Color::WHITE}),
		CostumeMapper(CssCharacter::FALCO, {Color::WHITE, Color::RED, Color::BLUE, Color::GREEN}),
		CostumeMapper(CssCharacter::YOUNG_LINK, {Color::GREEN, Color::RED, Color::BLUE, Color::WHITE, Color::BLACK}),
		CostumeMapper(CssCharacter::DR_MARIO, {Color::WHITE, Color::RED, Color::BLUE, Color::GREEN, Color::BLACK}),
		CostumeMapper(CssCharacter::ROY, {Color::PURPLE, Color::RED, Color::BLUE, Color::GREEN, Color::YELLOW}),
		CostumeMapper(CssCharacter::PICHU, {Color::YELLOW, Color::RED, Color::BLUE, Color::GREEN}),
		CostumeMapper(CssCharacter::GANONDORF, {Color::BROWN, Color::RED, Color::BLUE, Color::GREEN, Color::PURPLE}),
		CostumeMapper(CssCharacter::MASTER_HAND, {}),
		CostumeMapper(CssCharacter::WIREFRAME_MALE, {}),
		CostumeMapper(CssCharacter::WIREFRAME_FEMALE, {}),
		CostumeMapper(CssCharacter::GIGA_BOWSER, {}),
		CostumeMapper(CssCharacter::CRAZY_HAND, {}),
		CostumeMapper(CssCharacter::SANDBAG, {}),
		CostumeMapper(CssCharacter::POPO, {}),
	};

	return kMappers.at(character);
}

CssCharacter CostumeMapper::character() const {
	return character_;
}

Color CostumeMapper::toColor(Costume costume) const {
	if (static_cast<uint8_t>(costume) >= Costume::kMaxNumCostumes) {
		return Color::INVALID;
	}
	return toColor_[costume];
}

Costume CostumeMapper::toCostume(Color color) const {
	int8_t colorValue = color;
	if (colorValue <= Color::INVALID || colorValue > Color::NAVY) {
		throw NoSuchColorException(
			std::format("Color with value {} does not exist.", colorValue));
	} else if (toCostume_[color] == static_cast<Costume>(-1)) {
		throw NoSuchColorException(
			std::format("Color {} does not exist for {}.", color.toString(), character_.toString()));
	}
	return toCostume_[color];
}

size_t CostumeMapper::numCostumes() const {
	return numCostumes_;
}


}  // namespace cpp_slippi

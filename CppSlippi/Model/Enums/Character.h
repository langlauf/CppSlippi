#pragma once

#include <cstdint>
#include <string_view>

#include "CppSlippi/Utils/Enums.h"


namespace cpp_slippi {

// Also called external character.
class CssCharacter {
	CPP_SLIPPI_DEFINE_ENUM(CssCharacter, uint8_t,
		CAPTAIN_FALCON = 0,
		DONKEY_KONG = 1,
		FOX = 2,
		GAME_AND_WATCH = 3,
		KIRBY = 4,
		BOWSER = 5,
		LINK = 6,
		LUIGI = 7,
		MARIO = 8,
		MARTH = 9,
		MEWTWO = 10,
		NESS = 11,
		PEACH = 12,
		PIKACHU = 13,
		ICE_CLIMBERS = 14,
		JIGGLYPUFF = 15,
		SAMUS = 16,
		YOSHI = 17,
		ZELDA = 18,
		SHEIK = 19,
		FALCO = 20,
		YOUNG_LINK = 21,
		DR_MARIO = 22,
		ROY = 23,
		PICHU = 24,
		GANONDORF = 25,
		MASTER_HAND = 26,
		WIREFRAME_MALE = 27,
		WIREFRAME_FEMALE = 28,
		GIGA_BOWSER = 29,
		CRAZY_HAND = 30,
		SANDBAG = 31,
		POPO = 32);

public:
	constexpr std::string_view toString() const {
		switch(value) {
			case CAPTAIN_FALCON:
				return "Captain Falcon";
			case DONKEY_KONG:
				return "Donkey Kong";
			case FOX:
				return "Fox";
			case GAME_AND_WATCH:
				return "Game And Watch";
			case KIRBY:
				return "Kirby";
			case BOWSER:
				return "Bowser";
			case LINK:
				return "Link";
			case LUIGI:
				return "Luigi";
			case MARIO:
				return "Mario";
			case MARTH:
				return "Marth";
			case MEWTWO:
				return "Mewtwo";
			case NESS:
				return "Ness";
			case PEACH:
				return "Peach";
			case PIKACHU:
				return "Pikachu";
			case ICE_CLIMBERS:
				return "Ice Climbers";
			case JIGGLYPUFF:
				return "Jigglypuff";
			case SAMUS:
				return "Samus";
			case YOSHI:
				return "Yoshi";
			case ZELDA:
				return "Zelda";
			case SHEIK:
				return "Sheik";
			case FALCO:
				return "Falco";
			case YOUNG_LINK:
				return "Young Link";
			case DR_MARIO:
				return "Dr Mario";
			case ROY:
				return "Roy";
			case PICHU:
				return "Pichu";
			case GANONDORF:
				return "Ganondorf";
			case MASTER_HAND:
				return "Master Hand";
			case WIREFRAME_MALE:
				return "Wireframe Male";
			case WIREFRAME_FEMALE:
				return "Wireframe Female";
			case GIGA_BOWSER:
				return "Giga Bowser";
			case CRAZY_HAND:
				return "Crazy Hand";
			case SANDBAG:
				return "Sandbag";
			case POPO:
				return "Popo";
			default:
				return "Unknown";
		}
	}
};


// Also called internal character.
class InGameCharacter {
	CPP_SLIPPI_DEFINE_ENUM(InGameCharacter, uint8_t,
		MARIO = 0,
		FOX = 1,
		CAPTAIN_FALCON = 2,
		DONKEY_KONG = 3,
		KIRBY = 4,
		BOWSER = 5,
		LINK = 6,
		SHEIK = 7,
		NESS = 8,
		PEACH = 9,
		POPO = 10,
		NANA = 11,
		PIKACHU = 12,
		SAMUS = 13,
		YOSHI = 14,
		JIGGLYPUFF = 15,
		MEWTWO = 16,
		LUIGI = 17,
		MARTH = 18,
		ZELDA = 19,
		YOUNG_LINK = 20,
		DR_MARIO = 21,
		FALCO = 22,
		PICHU = 23,
		GAME_AND_WATCH = 24,
		GANONDORF = 25,
		ROY = 26,
		MASTER_HAND = 27,
		CRAZY_HAND = 28,
		WIREFRAME_MALE = 29,
		WIREFRAME_FEMALE = 30,
		GIGA_BOWSER = 31,
		SANDBAG = 32);

public:
	constexpr const std::string_view toString() const {
		switch (value) {
			case MARIO:
				return "Mario";
			case FOX:
				return "Fox";
			case CAPTAIN_FALCON:
				return "Captain Falcon";
			case DONKEY_KONG:
				return "Donkey Kong";
			case KIRBY:
				return "Kirby";
			case BOWSER:
				return "Bowser";
			case LINK:
				return "Link";
			case SHEIK:
				return "Sheik";
			case NESS:
				return "Ness";
			case PEACH:
				return "Peach";
			case POPO:
				return "Popo";
			case NANA:
				return "Nana";
			case PIKACHU:
				return "Pikachu";
			case SAMUS:
				return "Samus";
			case YOSHI:
				return "Yoshi";
			case JIGGLYPUFF:
				return "Jigglypuff";
			case MEWTWO:
				return "Mewtwo";
			case LUIGI:
				return "Luigi";
			case MARTH:
				return "Marth";
			case ZELDA:
				return "Zelda";
			case YOUNG_LINK:
				return "Young Link";
			case DR_MARIO:
				return "Dr Mario";
			case FALCO:
				return "Falco";
			case PICHU:
				return "Pichu";
			case GAME_AND_WATCH:
				return "Game And Watch";
			case GANONDORF:
				return "Ganondorf";
			case ROY:
				return "Roy";
			case MASTER_HAND:
				return "Master Hand";
			case CRAZY_HAND:
				return "Crazy Hand";
			case WIREFRAME_MALE:
				return "Wireframe Male";
			case WIREFRAME_FEMALE:
				return "Wireframe Female";
			case GIGA_BOWSER:
				return "Giga Bowser";
			case SANDBAG:
				return "Sandbag";
			default:
				return "Unknown";
		}
	}
};

}  // namespace cpp_slippi

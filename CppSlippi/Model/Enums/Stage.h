#pragma once

#include <cstdint>
#include <string_view>

#include "CppSlippi/Utils/Enums.h"


namespace cpp_slippi {

class Stage {
	CPP_SLIPPI_DEFINE_ENUM(Stage, uint16_t,
		FOUNTAIN_OF_DREAMS = 2,
		POKEMON_STADIUM = 3,
		PRINCESS_PEACHS_CASTLE = 4,
		KONGO_JUNGLE = 5,
		BRINSTAR = 6,
		CORNERIA = 7,
		YOSHIS_STORY = 8,
		ONETT = 9,
		MUTE_CITY = 10,
		RAINBOW_CRUISE = 11,
		JUNGLE_JAPES = 12,
		GREAT_BAY = 13,
		HYRULE_TEMPLE = 14,
		BRINSTAR_DEPTHS = 15,
		YOSHIS_ISLAND = 16,
		GREEN_GREENS = 17,
		FOURSIDE = 18,
		MUSHROOM_KINGDOM_I = 19,
		MUSHROOM_KINGDOM_II = 20,
		VENOM = 22,
		POKE_FLOATS = 23,
		BIG_BLUE = 24,
		ICICLE_MOUNTAIN = 25,
		ICETOP = 26,
		FLAT_ZONE = 27,
		DREAM_LAND_N64 = 28,
		YOSHIS_ISLAND_N64 = 29,
		KONGO_JUNGLE_N64 = 30,
		BATTLEFIELD = 31,
		FINAL_DESTINATION = 32);

public:
	constexpr std::string_view toString() const {
		switch(value) {
			case FOUNTAIN_OF_DREAMS:
				return "Fountain of Dreams";
			case POKEMON_STADIUM:
				return "Pokémon Stadium";
			case PRINCESS_PEACHS_CASTLE:
				return "Princess Peach's Castle";
			case KONGO_JUNGLE:
				return "Kongo Jungle";
			case BRINSTAR:
				return "Brinstar";
			case CORNERIA:
				return "Corneria";
			case YOSHIS_STORY:
				return "Yoshi's Story";
			case ONETT:
				return "Onett";
			case MUTE_CITY:
				return "Mute City";
			case RAINBOW_CRUISE:
				return "Rainbow Cruise";
			case JUNGLE_JAPES:
				return "Jungle Japes";
			case GREAT_BAY:
				return "Great Bay";
			case HYRULE_TEMPLE:
				return "Hyrule Temple";
			case BRINSTAR_DEPTHS:
				return "Brinstar Depths";
			case YOSHIS_ISLAND:
				return "Yoshi's Island";
			case GREEN_GREENS:
				return "Green Greens";
			case FOURSIDE:
				return "Fourside";
			case MUSHROOM_KINGDOM_I:
				return "Mushroom Kingdom I";
			case MUSHROOM_KINGDOM_II:
				return "Mushroom Kingdom II";
			case VENOM:
				return "Venom";
			case POKE_FLOATS:
				return "Poké Floats";
			case BIG_BLUE:
				return "Big Blue";
			case ICICLE_MOUNTAIN:
				return "Icicle Mountain";
			case ICETOP:
				return "Icetop";
			case FLAT_ZONE:
				return "Flat Zone";
			case DREAM_LAND_N64:
				return "Dream Land N64";
			case YOSHIS_ISLAND_N64:
				return "Yoshi's Island N64";
			case KONGO_JUNGLE_N64:
				return "Kongo Jungle N64";
			case BATTLEFIELD:
				return "Battlefield";
			case FINAL_DESTINATION:
				return "Final Destination";
			default:
				return "Unknown";
		}
	}
};

}  // namespace cpp_slippi

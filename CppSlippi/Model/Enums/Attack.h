#pragma once

#include <cstdint>
#include <string_view>

#include "CppSlippi/Utils/Enums.h"


namespace cpp_slippi {

class Attack {
	DEFINE_ENUM(Attack, uint8_t,
		NON_STALING = 1,
		JAB_1 = 2,
		JAB_2 = 3,
		JAB_3 = 4,
		RAPID_JABS = 5,
		DASH_ATTACK = 6,
		SIDE_TILT = 7,
		UP_TILT = 8,
		DOWN_TILT = 9,
		SIDE_SMASH = 10,
		UP_SMASH = 11,
		DOWN_SMASH = 12,
		NAIR = 13,
		FAIR = 14,
		BAIR = 15,
		UAIR = 16,
		DAIR = 17,
		NEUTRAL_SPECIAL = 18,
		SIDE_SPECIAL = 19,
		UP_SPECIAL = 20,
		DOWN_SPECIAL = 21,
		KIRBY_HAT_MARIO_NEUTRAL_SPECIAL = 22,
		KIRBY_HAT_FOX_NEUTRAL_SPECIAL = 23,
		KIRBY_HAT_CFALCON_NEUTRAL_SPECIAL = 24,
		KIRBY_HAT_DKNEUTRAL_SPECIAL = 25,
		KIRBY_HAT_BOWSER_NEUTRAL_SPECIAL = 26,
		KIRBY_HAT_LINK_NEUTRAL_SPECIAL = 27,
		KIRBY_HAT_SHEIK_NEUTRAL_SPECIAL = 28,
		KIRBY_HAT_NESS_NEUTRAL_SPECIAL = 29,
		KIRBY_HAT_PEACH_NEUTRAL_SPECIAL = 30,
		KIRBY_HAT_ICE_CLIMBER_NEUTRAL_SPECIAL = 31,
		KIRBY_HAT_PIKACHU_NEUTRAL_SPECIAL = 32,
		KIRBY_HAT_SAMUS_NEUTRAL_SPECIAL = 33,
		KIRBY_HAT_YOSHI_NEUTRAL_SPECIAL = 34,
		KIRBY_HAT_JIGGLYPUFF_NEUTRAL_SPECIAL = 35,
		KIRBY_HAT_MEWTWO_NEUTRAL_SPECIAL = 36,
		KIRBY_HAT_LUIGI_NEUTRAL_SPECIAL = 37,
		KIRBY_HAT_MARTH_NEUTRAL_SPECIAL = 38,
		KIRBY_HAT_ZELDA_NEUTRAL_SPECIAL = 39,
		KIRBY_HAT_YOUNG_LINK_NEUTRAL_SPECIAL = 40,
		KIRBY_HAT_DOC_NEUTRAL_SPECIAL = 41,
		KIRBY_HAT_FALCO_NEUTRAL_SPECIAL = 42,
		KIRBY_HAT_PICHU_NEUTRAL_SPECIAL = 43,
		KIRBY_HAT_GAME_AND_WATCH_NEUTRAL_SPECIAL = 44,
		KIRBY_HAT_GANON_NEUTRAL_SPECIAL = 45,
		KIRBY_HAT_ROY_NEUTRAL_SPECIAL = 46,
		GET_UP_ATTACK_FROM_BACK = 50,
		GET_UP_ATTACK_FROM_FRONT = 51,
		PUMMEL = 52,
		FORWARD_THROW = 53,
		BACK_THROW = 54,
		UP_THROW = 55,
		DOWN_THROW = 56,
		CARGO_FORWARD_THROW = 57,
		CARGO_BACK_THROW = 58,
		CARGO_UP_THROW = 59,
		CARGO_DOWN_THROW = 60,
		LEDGE_GET_UP_ATTACK_100 = 61,
		LEDGE_GET_UP_ATTACK = 62,
		BEAM_SWORD_JAB = 63,
		BEAM_SWORD_TILT_SWING = 64,
		BEAM_SWORD_SMASH_SWING = 65,
		BEAM_SWORD_DASH_SWING = 66,
		HOME_RUN_BAT_JAB = 67,
		HOME_RUN_BAT_TILT_SWING = 68,
		HOME_RUN_BAT_SMASH_SWING = 69,
		HOME_RUN_BAT_DASH_SWING = 70,
		PARASOL_JAB = 71,
		PARASOL_TILT_SWING = 72,
		PARASOL_SMASH_SWING = 73,
		PARASOL_DASH_SWING = 74,
		FAN_JAB = 75,
		FAN_TILT_SWING = 76,
		FAN_SMASH_SWING = 77,
		FAN_DASH_SWING = 78,
		STAR_ROD_JAB = 79,
		STAR_ROD_TILT_SWING = 80,
		STAR_ROD_SMASH_SWING = 81,
		STAR_ROD_DASH_SWING = 82,
		LIPS_STICK_JAB = 83,
		LIPS_STICK_TILT_SWING = 84,
		LIPS_STICK_SMASH_SWING = 85,
		LIPS_STICK_DASH_SWING = 86,
		OPEN_PARASOL = 87,
		RAY_GUN_SHOOT = 88,
		FIRE_FLOWER_SHOOT = 89,
		SCREW_ATTACK = 90,
		SUPER_SCOPE_RAPID = 91,
		SUPER_SCOPE_CHARGED = 92,
		HAMMER = 93);

public:
	constexpr std::string_view toString() const {
		switch (value) {
			case NON_STALING:
				return "Non Staling";
			case JAB_1:
				return "Jab 1";
			case JAB_2:
				return "Jab 2";
			case JAB_3:
				return "Jab 3";
			case RAPID_JABS:
				return "Rapid Jabs";
			case DASH_ATTACK:
				return "Dash Attack";
			case SIDE_TILT:
				return "Side Tilt";
			case UP_TILT:
				return "Up Tilt";
			case DOWN_TILT:
				return "Down Tilt";
			case SIDE_SMASH:
				return "Side Smash";
			case UP_SMASH:
				return "Up Smash";
			case DOWN_SMASH:
				return "Down Smash";
			case NAIR:
				return "Nair";
			case FAIR:
				return "Fair";
			case BAIR:
				return "Bair";
			case UAIR:
				return "Uair";
			case DAIR:
				return "Dair";
			case NEUTRAL_SPECIAL:
				return "Neutral Special";
			case SIDE_SPECIAL:
				return "Side Special";
			case UP_SPECIAL:
				return "Up Special";
			case DOWN_SPECIAL:
				return "Down Special";
			case KIRBY_HAT_MARIO_NEUTRAL_SPECIAL:
				return "Kirby Hat Mario Neutral Special";
			case KIRBY_HAT_FOX_NEUTRAL_SPECIAL:
				return "Kirby Hat Fox Neutral Special";
			case KIRBY_HAT_CFALCON_NEUTRAL_SPECIAL:
				return "Kirby Hat Cfalcon Neutral Special";
			case KIRBY_HAT_DKNEUTRAL_SPECIAL:
				return "Kirby Hat Dkneutral Special";
			case KIRBY_HAT_BOWSER_NEUTRAL_SPECIAL:
				return "Kirby Hat Bowser Neutral Special";
			case KIRBY_HAT_LINK_NEUTRAL_SPECIAL:
				return "Kirby Hat Link Neutral Special";
			case KIRBY_HAT_SHEIK_NEUTRAL_SPECIAL:
				return "Kirby Hat Sheik Neutral Special";
			case KIRBY_HAT_NESS_NEUTRAL_SPECIAL:
				return "Kirby Hat Ness Neutral Special";
			case KIRBY_HAT_PEACH_NEUTRAL_SPECIAL:
				return "Kirby Hat Peach Neutral Special";
			case KIRBY_HAT_ICE_CLIMBER_NEUTRAL_SPECIAL:
				return "Kirby Hat Ice Climber Neutral Special";
			case KIRBY_HAT_PIKACHU_NEUTRAL_SPECIAL:
				return "Kirby Hat Pikachu Neutral Special";
			case KIRBY_HAT_SAMUS_NEUTRAL_SPECIAL:
				return "Kirby Hat Samus Neutral Special";
			case KIRBY_HAT_YOSHI_NEUTRAL_SPECIAL:
				return "Kirby Hat Yoshi Neutral Special";
			case KIRBY_HAT_JIGGLYPUFF_NEUTRAL_SPECIAL:
				return "Kirby Hat Jigglypuff Neutral Special";
			case KIRBY_HAT_MEWTWO_NEUTRAL_SPECIAL:
				return "Kirby Hat Mewtwo Neutral Special";
			case KIRBY_HAT_LUIGI_NEUTRAL_SPECIAL:
				return "Kirby Hat Luigi Neutral Special";
			case KIRBY_HAT_MARTH_NEUTRAL_SPECIAL:
				return "Kirby Hat Marth Neutral Special";
			case KIRBY_HAT_ZELDA_NEUTRAL_SPECIAL:
				return "Kirby Hat Zelda Neutral Special";
			case KIRBY_HAT_YOUNG_LINK_NEUTRAL_SPECIAL:
				return "Kirby Hat Young Link Neutral Special";
			case KIRBY_HAT_DOC_NEUTRAL_SPECIAL:
				return "Kirby Hat Doc Neutral Special";
			case KIRBY_HAT_FALCO_NEUTRAL_SPECIAL:
				return "Kirby Hat Falco Neutral Special";
			case KIRBY_HAT_PICHU_NEUTRAL_SPECIAL:
				return "Kirby Hat Pichu Neutral Special";
			case KIRBY_HAT_GAME_AND_WATCH_NEUTRAL_SPECIAL:
				return "Kirby Hat Game And Watch Neutral Special";
			case KIRBY_HAT_GANON_NEUTRAL_SPECIAL:
				return "Kirby Hat Ganon Neutral Special";
			case KIRBY_HAT_ROY_NEUTRAL_SPECIAL:
				return "Kirby Hat Roy Neutral Special";
			case GET_UP_ATTACK_FROM_BACK:
				return "Get Up Attack From Back";
			case GET_UP_ATTACK_FROM_FRONT:
				return "Get Up Attack From Front";
			case PUMMEL:
				return "Pummel";
			case FORWARD_THROW:
				return "Forward Throw";
			case BACK_THROW:
				return "Back Throw";
			case UP_THROW:
				return "Up Throw";
			case DOWN_THROW:
				return "Down Throw";
			case CARGO_FORWARD_THROW:
				return "Cargo Forward Throw";
			case CARGO_BACK_THROW:
				return "Cargo Back Throw";
			case CARGO_UP_THROW:
				return "Cargo Up Throw";
			case CARGO_DOWN_THROW:
				return "Cargo Down Throw";
			case LEDGE_GET_UP_ATTACK_100:
				return "Ledge Get Up Attack 100";
			case LEDGE_GET_UP_ATTACK:
				return "Ledge Get Up Attack";
			case BEAM_SWORD_JAB:
				return "Beam Sword Jab";
			case BEAM_SWORD_TILT_SWING:
				return "Beam Sword Tilt Swing";
			case BEAM_SWORD_SMASH_SWING:
				return "Beam Sword Smash Swing";
			case BEAM_SWORD_DASH_SWING:
				return "Beam Sword Dash Swing";
			case HOME_RUN_BAT_JAB:
				return "Home Run Bat Jab";
			case HOME_RUN_BAT_TILT_SWING:
				return "Home Run Bat Tilt Swing";
			case HOME_RUN_BAT_SMASH_SWING:
				return "Home Run Bat Smash Swing";
			case HOME_RUN_BAT_DASH_SWING:
				return "Home Run Bat Dash Swing";
			case PARASOL_JAB:
				return "Parasol Jab";
			case PARASOL_TILT_SWING:
				return "Parasol Tilt Swing";
			case PARASOL_SMASH_SWING:
				return "Parasol Smash Swing";
			case PARASOL_DASH_SWING:
				return "Parasol Dash Swing";
			case FAN_JAB:
				return "Fan Jab";
			case FAN_TILT_SWING:
				return "Fan Tilt Swing";
			case FAN_SMASH_SWING:
				return "Fan Smash Swing";
			case FAN_DASH_SWING:
				return "Fan Dash Swing";
			case STAR_ROD_JAB:
				return "Star Rod Jab";
			case STAR_ROD_TILT_SWING:
				return "Star Rod Tilt Swing";
			case STAR_ROD_SMASH_SWING:
				return "Star Rod Smash Swing";
			case STAR_ROD_DASH_SWING:
				return "Star Rod Dash Swing";
			case LIPS_STICK_JAB:
				return "Lips Stick Jab";
			case LIPS_STICK_TILT_SWING:
				return "Lips Stick Tilt Swing";
			case LIPS_STICK_SMASH_SWING:
				return "Lips Stick Smash Swing";
			case LIPS_STICK_DASH_SWING:
				return "Lips Stick Dash Swing";
			case OPEN_PARASOL:
				return "Open Parasol";
			case RAY_GUN_SHOOT:
				return "Ray Gun Shoot";
			case FIRE_FLOWER_SHOOT:
				return "Fire Flower Shoot";
			case SCREW_ATTACK:
				return "Screw Attack";
			case SUPER_SCOPE_RAPID:
				return "Super Scope Rapid";
			case SUPER_SCOPE_CHARGED:
				return "Super Scope Charged";
			case HAMMER:
				return "Hammer";
			default:
				return "Unknown";
		}
	}
};

}  // namespace cpp_slippi

#pragma once

#include <cstdint>
#include <string_view>

#include "CppSlippi/Utils/Enums.h"
#include "CppSlippi/Utils/BitFields.h"


namespace cpp_slippi {

class ItemType {
	CPP_SLIPPI_DEFINE_ENUM(ItemType, uint16_t,
		CAPSULE = 0,
		BOX = 1,
		BARREL = 2,
		EGG = 3,
		PARTY_BALL = 4,
		BARREL_CANNON = 5,
		BOB_OMB = 6,
		MR_SATURN = 7,
		HEART_CONTAINER = 8,
		MAXIM_TOMATO = 9,
		STARMAN = 10,
		HOME_RUN_BAT = 11,
		BEAM_SWORD = 12,
		PARASOL = 13,
		GREEN_SHELL_1 = 14,
		RED_SHELL_1 = 15,
		RAY_GUN = 16,
		FREEZIE = 17,
		FOOD = 18,
		PROXIMITY_MINE = 19,
		FLIPPER = 20,
		SUPER_SCOPE = 21,
		STAR_ROD = 22,
		LIPS_STICK = 23,
		FAN = 24,
		FIRE_FLOWER = 25,
		SUPER_MUSHROOM = 26,
		WARP_STAR = 29,
		SCREW_ATTACK = 30,
		BUNNY_HOOD = 31,
		METAL_BOX = 32,
		CLOAKING_DEVICE = 33,
		POKE_BALL = 34,

		// Item-related
		RAY_GUN_RECOIL = 35,
		STAR_ROD_STAR = 36,
		LIPS_STICK_DUST = 37,
		SUPER_SCOPE_BEAM = 38,
		RAY_GUN_BEAM = 39,
		HAMMER_HEAD = 40,
		FLOWER = 41,
		YOSHI_EGG_1 = 42,

		// Monsters
		GOOMBA = 43,
		REDEAD = 44,
		OCTAROK = 45,
		OTTOSEA = 46,
		STONE = 47,

		// Character-related
		MARIO_FIRE = 48,
		DR_MARIO_PILL = 49,
		KIRBY_CUTTER_BEAM = 50,
		KIRBY_HAMMER = 51,
		FOX_LASER = 54,
		FALCO_LASER = 55,
		FOX_SHADOW = 56,
		FALCO_SHADOW = 57,
		LINK_BOMB = 58,
		YOUNG_LINK_BOMB = 59,
		LINK_BOOMERANG = 60,
		YOUNG_LINK_BOOMERANG = 61,
		LINK_HOOKSHOT = 62,
		YOUNG_LINK_HOOKSHOT = 63,
		LINK_ARROW_1 = 64,
		YOUNG_LINK_FIRE_ARROW = 65,
		NESS_PK_FIRE = 66,
		NESS_PK_FLASH_1 = 67,
		NESS_PK_FLASH_2 = 68,
		NESS_PK_THUNDER_1 = 69,
		NESS_PK_THUNDER_2 = 70,
		NESS_PK_THUNDER_3 = 71,
		NESS_PK_THUNDER_4 = 72,
		NESS_PK_THUNDER_5 = 73,
		FOX_BLASTER = 74,
		FALCO_BLASTER = 75,
		LINK_ARROW_2 = 76,
		YOUNG_LINK_ARROW = 77,
		NESS_PK_FLASH_3 = 78,
		SHEIK_NEEDLE_1 = 79,
		SHEIK_NEEDLE_2 = 80,
		PIKACHU_THUNDER_1 = 81,
		PICHU_THUNDER_1 = 82,
		MARIO_CAPE = 83,
		DR_MARIO_CAPE = 84,
		SHEIK_SMOKE = 85,
		YOSHI_EGG_2 = 86,
		YOSHI_TONGUE_1 = 87,
		YOSHI_STAR = 88,
		PIKACHU_THUNDER_2 = 89,
		PIKACHU_THUNDER_3 = 90,
		PICHU_THUNDER_2 = 91,
		PICHU_THUNDER_3 = 92,
		SAMUS_BOMB = 93,
		SAMUS_CHARGE_SHOT = 94,
		SAMUS_MISSILE = 95,
		SAMUS_GRAPPLE_BEAM = 96,
		SHEIK_CHAIN = 97,
		PEACH_TURNIP = 99,
		BOWSER_FLAME = 100,
		NESS_BAT = 101,
		NESS_YOYO = 102,
		PEACH_PARASOL = 103,
		PEACH_TOAD = 104,
		LUIGI_FIRE = 105,
		ICE_CLIMBERS_ICE = 106,
		ICE_CLIMBERS_BLIZZARD = 107,
		ZELDA_FIRE_1 = 108,
		ZELDA_FIRE_2 = 109,
		PEACH_TOAD_SPORE = 111,
		MEWTWO_SHADOW_BALL = 112,
		ICE_CLIMBERS_UP_B = 113,
		GAME_AND_WATCH_PESTICIDE = 114,
		GAME_AND_WATCH_MANHOLE = 115,
		GAME_AND_WATCH_FIRE = 116,
		GAME_AND_WATCH_PARACHUTE = 117,
		GAME_AND_WATCH_TURTLE = 118,
		GAME_AND_WATCH_SPERKY = 119,
		GAME_AND_WATCH_JUDGE = 120,
		GAME_AND_WATCH_SAUSAGE = 122,
		GAME_AND_WATCH_MILK = 123,
		GAME_AND_WATCH_FIREFIGHTER = 124,
		MASTER_HAND_LASER = 125,
		MASTER_HAND_BULLET = 126,
		CRAZY_HAND_LASER = 127,
		CRAZY_HAND_BULLET = 128,
		CRAZY_HAND_BOMB = 129,
		KIRBY_COPY_MARIO_FIRE = 130,
		KIRBY_COPY_DR_MARIO_PILL = 131,
		KIRBY_COPY_LUIGI_FIRE = 132,
		KIRBY_COPY_ICE_CLIMBERS_ICE = 133,
		KIRBY_COPY_PEACH_TOAD = 134,
		KIRBY_COPY_TOAD_SPORE = 135,
		KIRBY_COPY_FOX_LASER = 136,
		KIRBY_COPY_FALCO_LASER = 137,
		KIRBY_COPY_FOX_BLASTER = 138,
		KIRBY_COPY_FALCO_BLASTER = 139,
		KIRBY_COPY_LINK_ARROW_1 = 140,
		KIRBY_COPY_YOUNG_LINK_ARROW_1 = 141,
		KIRBY_COPY_LINK_ARROW_2 = 142,
		KIRBY_COPY_YOUNG_LINK_ARROW_2 = 143,
		KIRBY_COPY_MEWTWO_SHADOW_BALL = 144,
		KIRBY_COPY_PK_FLASH = 145,
		KIRBY_COPY_PK_FLASH_EXPLOSION = 146,
		KIRBY_COPY_PIKACHU_THUNDER_1 = 147,
		KIRBY_COPY_PIKACHU_THUNDER_2 = 148,
		KIRBY_COPY_PICHU_THUNDER_1 = 149,
		KIRBY_COPY_PICHU_THUNDER_2 = 150,
		KIRBY_COPY_SAMUS_CHARGE_SHOT = 151,
		KIRBY_COPY_SHEIK_NEEDLE_1 = 152,
		KIRBY_COPY_SHEIK_NEEDLE_2 = 153,
		KIRBY_COPY_BOWSER_FLAME = 154,
		KIRBY_COPY_GAME_AND_WATCH_SAUSAGE = 155,
		YOSHI_TONGUE_2 = 157,
		MARIO_LUIGI_COIN = 159,

		// Pokemon
		RANDOM_POKEMON = 160,
		GOLDEEN = 161,
		CHICORITA = 162,
		SNORLAX = 163,
		BLASTOISE = 164,
		WEEZING = 165,
		CHARIZARD = 166,
		MOLTRES = 167,
		ZAPDOS = 168,
		ARTICUNO = 169,
		WOBBUFFET = 170,
		SCIZOR = 171,
		UNOWN = 172,
		ENTEI = 173,
		RAIKOU = 174,
		SUICUNE = 175,
		BELLOSSOM = 176,
		ELECTRODE = 177,
		LUGIA = 178,
		HO_OH = 179,
		DITTO = 180,
		CLEFAIRY = 181,
		TOGEPI = 182,
		MEW = 183,
		CELEBI = 184,
		STARYU = 185,
		CHANSEY = 186,
		PORYGON = 187,
		CYNDAQUIL = 188,
		MARILL = 189,
		VENUSAUR = 190,

		// Pokemon-related
		CHICORITA_LEAF = 191,
		BLASTOISE_WATER = 192,
		WEEZING_GAS_1 = 193,
		WEEZING_GAS_2 = 194,
		CHARIZARD_BREATH_1 = 195,
		CHARIZARD_BREATH_2 = 196,
		CHARIZARD_BREATH_3 = 197,
		CHARIZARD_BREATH_4 = 198,
		MINI_UNOWNS = 199,
		LUGIA_AEROBLAST_1 = 200,
		LUGIA_AEROBLAST_2 = 201,
		LUGIA_AEROBLAST_3 = 202,
		HO_OH_FLAME = 203,
		STARYU_STAR = 204,
		HEALING_EGG = 205,
		CYNDAQUIL_FIRE = 206,

		// Monsters
		OLD_GOOMBA = 208,
		TARGET = 209,
		SHYGUY = 210,
		KOOPA_1 = 211,
		KOOPA_2 = 212,
		LIKE_LIKE = 213,
		OLD_OTTOSEA = 216,
		WHITE_BEAR = 217,
		KLAP = 218,
		GREEN_SHELL_2 = 219,
		RED_SHELL_2 = 220,

		// Stage-specific
		TINGLE = 221,
		APPLE = 225,
		HEALING_APPLE = 226,
		TOOL = 230,
		BIRDO = 233,
		ARWING_LASER = 234,
		GREAT_FOX_LASER = 235,
		BIRDO_EGG = 236);

public:
	constexpr std::string_view toString() const {
		switch (value) {
			case CAPSULE:
				return "Capsule";
			case BOX:
				return "Box";
			case BARREL:
				return "Barrel";
			case EGG:
				return "Egg";
			case PARTY_BALL:
				return "Party Ball";
			case BARREL_CANNON:
				return "Barrel Cannon";
			case BOB_OMB:
				return "Bob Omb";
			case MR_SATURN:
				return "Mr Saturn";
			case HEART_CONTAINER:
				return "Heart Container";
			case MAXIM_TOMATO:
				return "Maxim Tomato";
			case STARMAN:
				return "Starman";
			case HOME_RUN_BAT:
				return "Home Run Bat";
			case BEAM_SWORD:
				return "Beam Sword";
			case PARASOL:
				return "Parasol";
			case GREEN_SHELL_1:
				return "Green Shell 1";
			case RED_SHELL_1:
				return "Red Shell 1";
			case RAY_GUN:
				return "Ray Gun";
			case FREEZIE:
				return "Freezie";
			case FOOD:
				return "Food";
			case PROXIMITY_MINE:
				return "Proximity Mine";
			case FLIPPER:
				return "Flipper";
			case SUPER_SCOPE:
				return "Super Scope";
			case STAR_ROD:
				return "Star Rod";
			case LIPS_STICK:
				return "Lip's Stick";
			case FAN:
				return "Fan";
			case FIRE_FLOWER:
				return "Fire Flower";
			case SUPER_MUSHROOM:
				return "Super Mushroom";
			case WARP_STAR:
				return "Warp Star";
			case SCREW_ATTACK:
				return "Screw Attack";
			case BUNNY_HOOD:
				return "Bunny Hood";
			case METAL_BOX:
				return "Metal Box";
			case CLOAKING_DEVICE:
				return "Cloaking Device";
			case POKE_BALL:
				return "Poke Ball";
			case RAY_GUN_RECOIL:
				return "Ray Gun Recoil";
			case STAR_ROD_STAR:
				return "Star Rod Star";
			case LIPS_STICK_DUST:
				return "Lip's Stick Dust";
			case SUPER_SCOPE_BEAM:
				return "Super Scope Beam";
			case RAY_GUN_BEAM:
				return "Ray Gun Beam";
			case HAMMER_HEAD:
				return "Hammer Head";
			case FLOWER:
				return "Flower";
			case YOSHI_EGG_1:
				return "Yoshi Egg 1";
			case GOOMBA:
				return "Goomba";
			case REDEAD:
				return "Redead";
			case OCTAROK:
				return "Octarok";
			case OTTOSEA:
				return "Ottosea";
			case STONE:
				return "Stone";
			case MARIO_FIRE:
				return "Mario Fire";
			case DR_MARIO_PILL:
				return "Dr Mario Pill";
			case KIRBY_CUTTER_BEAM:
				return "Kirby Cutter Beam";
			case KIRBY_HAMMER:
				return "Kirby Hammer";
			case FOX_LASER:
				return "Fox Laser";
			case FALCO_LASER:
				return "Falco Laser";
			case FOX_SHADOW:
				return "Fox Shadow";
			case FALCO_SHADOW:
				return "Falco Shadow";
			case LINK_BOMB:
				return "Link Bomb";
			case YOUNG_LINK_BOMB:
				return "Young Link Bomb";
			case LINK_BOOMERANG:
				return "Link Boomerang";
			case YOUNG_LINK_BOOMERANG:
				return "Young Link Boomerang";
			case LINK_HOOKSHOT:
				return "Link Hookshot";
			case YOUNG_LINK_HOOKSHOT:
				return "Young Link Hookshot";
			case LINK_ARROW_1:
				return "Link Arrow 1";
			case YOUNG_LINK_FIRE_ARROW:
				return "Young Link Fire Arrow";
			case NESS_PK_FIRE:
				return "Ness Pk Fire";
			case NESS_PK_FLASH_1:
				return "Ness Pk Flash 1";
			case NESS_PK_FLASH_2:
				return "Ness Pk Flash 2";
			case NESS_PK_THUNDER_1:
				return "Ness Pk Thunder 1";
			case NESS_PK_THUNDER_2:
				return "Ness Pk Thunder 2";
			case NESS_PK_THUNDER_3:
				return "Ness Pk Thunder 3";
			case NESS_PK_THUNDER_4:
				return "Ness Pk Thunder 4";
			case NESS_PK_THUNDER_5:
				return "Ness Pk Thunder 5";
			case FOX_BLASTER:
				return "Fox Blaster";
			case FALCO_BLASTER:
				return "Falco Blaster";
			case LINK_ARROW_2:
				return "Link Arrow 2";
			case YOUNG_LINK_ARROW:
				return "Young Link Arrow";
			case NESS_PK_FLASH_3:
				return "Ness Pk Flash 3";
			case SHEIK_NEEDLE_1:
				return "Sheik Needle 1";
			case SHEIK_NEEDLE_2:
				return "Sheik Needle 2";
			case PIKACHU_THUNDER_1:
				return "Pikachu Thunder 1";
			case PICHU_THUNDER_1:
				return "Pichu Thunder 1";
			case MARIO_CAPE:
				return "Mario Cape";
			case DR_MARIO_CAPE:
				return "Dr Mario Cape";
			case SHEIK_SMOKE:
				return "Sheik Smoke";
			case YOSHI_EGG_2:
				return "Yoshi Egg 2";
			case YOSHI_TONGUE_1:
				return "Yoshi Tongue 1";
			case YOSHI_STAR:
				return "Yoshi Star";
			case PIKACHU_THUNDER_2:
				return "Pikachu Thunder 2";
			case PIKACHU_THUNDER_3:
				return "Pikachu Thunder 3";
			case PICHU_THUNDER_2:
				return "Pichu Thunder 2";
			case PICHU_THUNDER_3:
				return "Pichu Thunder 3";
			case SAMUS_BOMB:
				return "Samus Bomb";
			case SAMUS_CHARGE_SHOT:
				return "Samus Charge Shot";
			case SAMUS_MISSILE:
				return "Samus Missile";
			case SAMUS_GRAPPLE_BEAM:
				return "Samus Grapple Beam";
			case SHEIK_CHAIN:
				return "Sheik Chain";
			case PEACH_TURNIP:
				return "Peach Turnip";
			case BOWSER_FLAME:
				return "Bowser Flame";
			case NESS_BAT:
				return "Ness Bat";
			case NESS_YOYO:
				return "Ness Yoyo";
			case PEACH_PARASOL:
				return "Peach Parasol";
			case PEACH_TOAD:
				return "Peach Toad";
			case LUIGI_FIRE:
				return "Luigi Fire";
			case ICE_CLIMBERS_ICE:
				return "Ice Climbers Ice";
			case ICE_CLIMBERS_BLIZZARD:
				return "Ice Climbers Blizzard";
			case ZELDA_FIRE_1:
				return "Zelda Fire 1";
			case ZELDA_FIRE_2:
				return "Zelda Fire 2";
			case PEACH_TOAD_SPORE:
				return "Peach Toad Spore";
			case MEWTWO_SHADOW_BALL:
				return "Mewtwo Shadow Ball";
			case ICE_CLIMBERS_UP_B:
				return "Ice Climbers Up B";
			case GAME_AND_WATCH_PESTICIDE:
				return "Game And Watch Pesticide";
			case GAME_AND_WATCH_MANHOLE:
				return "Game And Watch Manhole";
			case GAME_AND_WATCH_FIRE:
				return "Game And Watch Fire";
			case GAME_AND_WATCH_PARACHUTE:
				return "Game And Watch Parachute";
			case GAME_AND_WATCH_TURTLE:
				return "Game And Watch Turtle";
			case GAME_AND_WATCH_SPERKY:
				return "Game And Watch Sperky";
			case GAME_AND_WATCH_JUDGE:
				return "Game And Watch Judge";
			case GAME_AND_WATCH_SAUSAGE:
				return "Game And Watch Sausage";
			case GAME_AND_WATCH_MILK:
				return "Game And Watch Milk";
			case GAME_AND_WATCH_FIREFIGHTER:
				return "Game And Watch Firefighter";
			case MASTER_HAND_LASER:
				return "Master Hand Laser";
			case MASTER_HAND_BULLET:
				return "Master Hand Bullet";
			case CRAZY_HAND_LASER:
				return "Crazy Hand Laser";
			case CRAZY_HAND_BULLET:
				return "Crazy Hand Bullet";
			case CRAZY_HAND_BOMB:
				return "Crazy Hand Bomb";
			case KIRBY_COPY_MARIO_FIRE:
				return "Kirby Copy Mario Fire";
			case KIRBY_COPY_DR_MARIO_PILL:
				return "Kirby Copy Dr Mario Pill";
			case KIRBY_COPY_LUIGI_FIRE:
				return "Kirby Copy Luigi Fire";
			case KIRBY_COPY_ICE_CLIMBERS_ICE:
				return "Kirby Copy Ice Climbers Ice";
			case KIRBY_COPY_PEACH_TOAD:
				return "Kirby Copy Peach Toad";
			case KIRBY_COPY_TOAD_SPORE:
				return "Kirby Copy Toad Spore";
			case KIRBY_COPY_FOX_LASER:
				return "Kirby Copy Fox Laser";
			case KIRBY_COPY_FALCO_LASER:
				return "Kirby Copy Falco Laser";
			case KIRBY_COPY_FOX_BLASTER:
				return "Kirby Copy Fox Blaster";
			case KIRBY_COPY_FALCO_BLASTER:
				return "Kirby Copy Falco Blaster";
			case KIRBY_COPY_LINK_ARROW_1:
				return "Kirby Copy Link Arrow 1";
			case KIRBY_COPY_YOUNG_LINK_ARROW_1:
				return "Kirby Copy Young Link Arrow 1";
			case KIRBY_COPY_LINK_ARROW_2:
				return "Kirby Copy Link Arrow 2";
			case KIRBY_COPY_YOUNG_LINK_ARROW_2:
				return "Kirby Copy Young Link Arrow 2";
			case KIRBY_COPY_MEWTWO_SHADOW_BALL:
				return "Kirby Copy Mewtwo Shadow Ball";
			case KIRBY_COPY_PK_FLASH:
				return "Kirby Copy Pk Flash";
			case KIRBY_COPY_PK_FLASH_EXPLOSION:
				return "Kirby Copy Pk Flash Explosion";
			case KIRBY_COPY_PIKACHU_THUNDER_1:
				return "Kirby Copy Pikachu Thunder 1";
			case KIRBY_COPY_PIKACHU_THUNDER_2:
				return "Kirby Copy Pikachu Thunder 2";
			case KIRBY_COPY_PICHU_THUNDER_1:
				return "Kirby Copy Pichu Thunder 1";
			case KIRBY_COPY_PICHU_THUNDER_2:
				return "Kirby Copy Pichu Thunder 2";
			case KIRBY_COPY_SAMUS_CHARGE_SHOT:
				return "Kirby Copy Samus Charge Shot";
			case KIRBY_COPY_SHEIK_NEEDLE_1:
				return "Kirby Copy Sheik Needle 1";
			case KIRBY_COPY_SHEIK_NEEDLE_2:
				return "Kirby Copy Sheik Needle 2";
			case KIRBY_COPY_BOWSER_FLAME:
				return "Kirby Copy Bowser Flame";
			case KIRBY_COPY_GAME_AND_WATCH_SAUSAGE:
				return "Kirby Copy Game And Watch Sausage";
			case YOSHI_TONGUE_2:
				return "Yoshi Tongue 2";
			case MARIO_LUIGI_COIN:
				return "Mario Luigi Coin";
			case RANDOM_POKEMON:
				return "Random Pokemon";
			case GOLDEEN:
				return "Goldeen";
			case CHICORITA:
				return "Chicorita";
			case SNORLAX:
				return "Snorlax";
			case BLASTOISE:
				return "Blastoise";
			case WEEZING:
				return "Weezing";
			case CHARIZARD:
				return "Charizard";
			case MOLTRES:
				return "Moltres";
			case ZAPDOS:
				return "Zapdos";
			case ARTICUNO:
				return "Articuno";
			case WOBBUFFET:
				return "Wobbuffet";
			case SCIZOR:
				return "Scizor";
			case UNOWN:
				return "Unown";
			case ENTEI:
				return "Entei";
			case RAIKOU:
				return "Raikou";
			case SUICUNE:
				return "Suicune";
			case BELLOSSOM:
				return "Bellossom";
			case ELECTRODE:
				return "Electrode";
			case LUGIA:
				return "Lugia";
			case HO_OH:
				return "Ho Oh";
			case DITTO:
				return "Ditto";
			case CLEFAIRY:
				return "Clefairy";
			case TOGEPI:
				return "Togepi";
			case MEW:
				return "Mew";
			case CELEBI:
				return "Celebi";
			case STARYU:
				return "Staryu";
			case CHANSEY:
				return "Chansey";
			case PORYGON:
				return "Porygon";
			case CYNDAQUIL:
				return "Cyndaquil";
			case MARILL:
				return "Marill";
			case VENUSAUR:
				return "Venusaur";
			case CHICORITA_LEAF:
				return "Chicorita Leaf";
			case BLASTOISE_WATER:
				return "Blastoise Water";
			case WEEZING_GAS_1:
				return "Weezing Gas 1";
			case WEEZING_GAS_2:
				return "Weezing Gas 2";
			case CHARIZARD_BREATH_1:
				return "Charizard Breath 1";
			case CHARIZARD_BREATH_2:
				return "Charizard Breath 2";
			case CHARIZARD_BREATH_3:
				return "Charizard Breath 3";
			case CHARIZARD_BREATH_4:
				return "Charizard Breath 4";
			case MINI_UNOWNS:
				return "Mini Unowns";
			case LUGIA_AEROBLAST_1:
				return "Lugia Aeroblast 1";
			case LUGIA_AEROBLAST_2:
				return "Lugia Aeroblast 2";
			case LUGIA_AEROBLAST_3:
				return "Lugia Aeroblast 3";
			case HO_OH_FLAME:
				return "Ho Oh Flame";
			case STARYU_STAR:
				return "Staryu Star";
			case HEALING_EGG:
				return "Healing Egg";
			case CYNDAQUIL_FIRE:
				return "Cyndaquil Fire";
			case OLD_GOOMBA:
				return "Old Goomba";
			case TARGET:
				return "Target";
			case SHYGUY:
				return "Shyguy";
			case KOOPA_1:
				return "Koopa 1";
			case KOOPA_2:
				return "Koopa 2";
			case LIKE_LIKE:
				return "Like Like";
			case OLD_OTTOSEA:
				return "Old Ottosea";
			case WHITE_BEAR:
				return "White Bear";
			case KLAP:
				return "Klap";
			case GREEN_SHELL_2:
				return "Green Shell 2";
			case RED_SHELL_2:
				return "Red Shell 2";
			case TINGLE:
				return "Tingle";
			case APPLE:
				return "Apple";
			case HEALING_APPLE:
				return "Healing Apple";
			case TOOL:
				return "Tool";
			case BIRDO:
				return "Birdo";
			case ARWING_LASER:
				return "Arwing Laser";
			case GREAT_FOX_LASER:
				return "Great Fox Laser";
			case BIRDO_EGG:
				return "Birdo Egg";
			default:
				return "Unknown";
		}
	}
};


// Bitfield for items that can spawn during gameplay and can be toggled in the
// options. See https://github.com/project-slippi/slippi-wiki/blob/master/SPEC.md#item-spawn-bitfield-1
class ItemSpawns {
	CPP_SLIPPI_DEFINE_ENUM(ItemSpawns, uint64_t,
		METAL_BOX = 1llu << 0,
		CLOAKING_DEVICE = 1llu << 1,
		POKEBALL = 1llu << 3,
		FAN = 1llu << 8,
		FIRE_FLOWER = 1llu << 9,
		SUPER_MUSHROOM = 1llu << 10,
		POISON_MUSHROOM = 1llu << 11,
		HAMMER = 1llu << 12,
		WARP_STAR = 1llu << 13,
		SCREW_ATTACK = 1llu << 14,
		BUNNY_HOOD = 1llu << 15,
		RAY_GUN = 1llu << 16,
		FREEZIE = 1llu << 17,
		FOOD = 1llu << 18,
		MOTION_SENSOR_BOMB = 1llu << 19,
		FLIPPER = 1llu << 20,
		SUPER_SCOPE = 1llu << 21,
		STAR_ROD = 1llu << 22,
		LIPS_STICK = 1llu << 23,
		HEART_CONTAINER = 1llu << 24,
		MAXIM_TOMATO = 1llu << 25,
		STARMAN = 1llu << 26,
		HOME_RUN_BAT = 1llu << 27,
		BEAM_SWORD = 1llu << 28,
		PARASOL = 1llu << 29,
		GREEN_SHELL = 1llu << 30,
		RED_SHELL = 1llu << 31,
		CAPSULE = 1llu << 32,
		BOX = 1llu << 33,
		BARREL = 1llu << 34,
		EGG = 1llu << 35,
		PARTY_BALL = 1llu << 36,
		BARREL_CANNON = 1llu << 37,
		BOB_OMB = 1llu << 38,
		MR_SATURN = 1llu << 39,

		ALL = 0xFFFFFFFFFF);

public:
	constexpr std::string_view toString() const {
		switch (value) {
			case METAL_BOX:
				return "MetalBox";
			case CLOAKING_DEVICE:
				return "CloakingDevice";
			case POKEBALL:
				return "Pokeball";
			case FAN:
				return "Fan";
			case FIRE_FLOWER:
				return "FireFlower";
			case SUPER_MUSHROOM:
				return "SuperMushroom";
			case POISON_MUSHROOM:
				return "PoisonMushroom";
			case HAMMER:
				return "Hammer";
			case WARP_STAR:
				return "WarpStar";
			case SCREW_ATTACK:
				return "ScrewAttack";
			case BUNNY_HOOD:
				return "BunnyHood";
			case RAY_GUN:
				return "RayGun";
			case FREEZIE:
				return "Freezie";
			case FOOD:
				return "Food";
			case MOTION_SENSOR_BOMB:
				return "MotionSensorBomb";
			case FLIPPER:
				return "Flipper";
			case SUPER_SCOPE:
				return "SuperScope";
			case STAR_ROD:
				return "StarRod";
			case LIPS_STICK:
				return "LipsStick";
			case HEART_CONTAINER:
				return "HeartContainer";
			case MAXIM_TOMATO:
				return "MaximTomato";
			case STARMAN:
				return "Starman";
			case HOME_RUN_BAT:
				return "HomeRunBat";
			case BEAM_SWORD:
				return "BeamSword";
			case PARASOL:
				return "Parasol";
			case GREEN_SHELL:
				return "GreenShell";
			case RED_SHELL:
				return "RedShell";
			case CAPSULE:
				return "Capsule";
			case BOX:
				return "Box";
			case BARREL:
				return "Barrel";
			case EGG:
				return "Egg";
			case PARTY_BALL:
				return "PartyBall";
			case BARREL_CANNON:
				return "BarrelCannon";
			case BOB_OMB:
				return "BobOmb";
			case MR_SATURN:
				return "MrSaturn";
			default:
				return "Unknown";
		}
	}
};
template<> constexpr bool is_bitfield<ItemSpawns::Enum> = true;


}  // namespace cpp_slippi

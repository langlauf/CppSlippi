#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>

#include "CppSlippi/Model/Enums/Character.h"
#include "CppSlippi/Utils/PortArray.h"


namespace cpp_slippi {


struct Metadata {
	struct Player {
		// Contains the number of frames for which a character was used. This is
		// mostly useful for Zelda/Sheik.
		//
		// Ex: {'18': 5209} means the player used Marth (internal character ID
		// 18) for all 5209 frames of the game
		std::unordered_map<InGameCharacter::Enum, uint32_t> characters;

		// Contains the Dolphin netplay name or Slippi Online Display Name
		// depending on which form of netplay was used. Will also contain the
		// connect code for Slippi Online if applicable.
		//
		// Ex: {'netplay': 'Player', 'code': 'ABCD#0'} means the player had the
		// Display Name Player and Connect Code ABCD#0. The netplay name from
		// Dolphin and Display Name from Slippi Online use the same key.
		std::unordered_map<std::string, std::string> names;

		friend bool operator==(const Player&, const Player&) = default;
	};

	// Timestamp of when the game started, ISO 8601 format
	// (e.g. 2018-06-22T07:52:59Z)
	std::optional<std::string> startAt;

	// The frame number of the last frame of the game. Used to show game
	// duration without parsing entire replay. Note that Peppi and Py-Slippi
	// use Duration here instead, however we choose to stick closer to the .slp
	// spec. The difference is 124 frames.
	std::optional<int32_t> lastFrame;

	// Metadata for the individual players.
	std::optional<utils::PortArray<std::optional<Player>>> players;

	// Platform the game was played on (values include dolphin, console, and network)
	std::optional<std::string> playedOn;

	// The name of the console the replay was created on.
	std::optional<std::string> consoleNick;

	friend constexpr bool operator==(const Metadata&, const Metadata&) = default;
};

}  // namespace cpp_slippi

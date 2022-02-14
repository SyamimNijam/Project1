#pragma once
#include <NGE/game.h>
#include <NB/nb.h>

struct settings {
	unsigned long zoom;
};

struct game_state {
	struct settings settings;
	int is_paused;

	unsigned long current_room_id;
};

enum direction {
	DIRECTION_S,
	DIRECTION_E,
	DIRECTION_N,
	DIRECTION_W,

	DIRECTION_DEFAULT = DIRECTION_S
};

// Load assets used by the game
int loadGameAssets();
// Release assets used by the game
void releaseGameAssets();

// Loads game save data from a file
int loadGameData(const char* file);
// Saves game data to a file
int saveGameData(const char* file);
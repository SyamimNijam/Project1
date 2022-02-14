#pragma once
#include <NGE/nge.h>

// Load necessary assets used by rooms
int loadRoomAssets();

enum room {
	ROOM_DEBUG,
	ROOM_DEBUG1,
	ROOM_BLANK,

	ROOM_COUNT
};

extern struct nge_room game_room[ROOM_COUNT];
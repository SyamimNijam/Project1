#pragma once
#include <NGE/nge.h>

// Load necessary assets used by objects
int loadObjectAssets();

enum object {
	OBJECT_CHAIR,
	OBJECT_COUNT
};

extern struct nge_object game_object[OBJECT_COUNT];
#pragma once
#include <NGE/nge.h>

// Load necessary assets used by entities
int loadEntityAssets();

enum direction getVectorDir(vector velocity);

enum entity {
	ENTITY_DDUDE,
	ENTITY_SLIME,
	ENTITY_COUNT
};

extern struct nge_entity game_entity[ENTITY_COUNT];
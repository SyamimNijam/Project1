#pragma once
#include "geometry.h"
#include "sprite.h"
#include <stdint.h>
#include <stdio.h>

struct nge_entity;
struct nge_room;

typedef struct nge_entity* self_entity;
typedef struct nge_entity* other_entity;

typedef void(*collision_func)(struct nge_room*,self_entity, other_entity);
typedef void(*update_func)(struct nge_room*,self_entity,milliseconds);
typedef void(*render_func)(struct nge_room*,self_entity);

typedef int bool;
#define true 1
#define false 0

// Static physics object
struct nge_object {
	unsigned long class_id;

	rectangle body;
	region collision; // Region relative to body center
	// diagonal > 0, 45 degree angle
	// diagonal < 0, 135 degree angle
	// diagonal = 0, regular box collisions
	int diagonal;
	// If this is false, entities can phase through object
	bool is_physical;

	struct sprite face;
};

// Entity that moves and interacts with other entities
struct nge_entity {
	unsigned long class_id;

	// Physics
	vector velocity;
	vector position_collect;
	rectangle body;
	region collision; // Region relative to body center

	// Flags
	//   If this->react_flags & other->collision_flags,
	//   entity will react to other entity
	unsigned long collision_flags;
	//   If this->react_flags & other->collision_flags,
	//   entity will react to other entity
	unsigned long react_flags;
	//   If this is false, entity can phase through objects
	bool is_physical;
	//   Flag is set when entity is touching an object
	bool is_touching;
	
	// Special data
	struct sprite face;
	size_t data_len;
	void* data;
	collision_func collide;
	update_func update;
	render_func render;
};

// Game level that has tiles, entities and objects as well as a camera position
struct nge_room {
	unsigned long id;
	vector camera;

	image tile_set;
	unsigned long horizontal_tile_count;
	unsigned long vertical_tile_count;
	const unsigned char* tile_map;

	struct nge_entity* entities;
	unsigned long entity_count;
	struct nge_object* objects;
	unsigned long object_count;
};

#define room_height(r) ((dimension)(r.tile_set.height * r.vertical_tile_count))
#define room_width(r) ((dimension)(r.tile_set.height * r.horizontal_tile_count))
// Updates all sprites and entities in room
void ngeUpdateRoom(struct nge_room*, milliseconds delta);
// Renders the entire room from the camera's perspective
void ngeRenderRoom(struct nge_room*, unsigned long scale);
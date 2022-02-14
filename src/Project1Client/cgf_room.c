#include "gamecontent.h"
#include "gameinput.h"
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "gamestate.h"
#include "cgf.h"
#include "rem.h"

void cgfRemoveEntity(struct nge_room* r, unsigned long i) {
	if (i < r->entity_count && r->entities != NULL) {
		if (r->entities[i].data_len && r->entities[i].data) {
			remFree(r->entities[i].data);
			r->entities[i].data = NULL;
		}
		r->entity_count -= 1;
		memmove(
			&r->entities[i],
			&r->entities[i + 1],
			sizeof(*r->entities) * (r->entity_count-i)
		);
	}
}

void cgfRemoveObject(struct nge_room* r, unsigned long i) {
	if (i < r->object_count && r->objects != NULL) {
		r->object_count -= 1;
		memmove(
			&r->objects[i],
			&r->objects[i + 1],
			sizeof(*r->objects) * (r->object_count - i)
		);
	}
}

static int insertItem(void** a, unsigned long* len, void* item, size_t item_size) {
	unsigned char* new_p = remRealloc(*a, (++ * len) * item_size);
	if (new_p) {
		*a = new_p;
		memcpy(&new_p[item_size * (*len - 1)], item, item_size);
		return 0;
	} else {
		(*len) -= 1;
		return 1;
	}
}

int cgfSpawnEntity(struct nge_room* r, enum entity entity_type, vector position) {
	struct nge_entity source = game_entity[entity_type];
	source.body.x = position.x;
	source.body.y = position.y;
	int ret = insertItem(
		(void**)&r->entities,
		&r->entity_count,
		(void*)&source,
		sizeof(struct nge_entity)
	);
	if (ret) return ret; else {
		struct nge_entity* src = &r->entities[r->entity_count - 1];
		src->data = remAlloc(src->data_len);
		if (src->data == NULL) return 2;
		memset(src->data, 0, src->data_len);
	}
	return 0;
}

int cgfSpawnObject(struct nge_room* r, enum object object_type, vector position) {
	struct nge_object source = game_object[object_type];
	source.body.x = position.x;
	source.body.y = position.y;
	return insertItem(
		(void**)&r->objects,
		&r->object_count,
		(void*)&source,
		sizeof(struct nge_object)
	);
}

void cgfMoveCamera(struct nge_room* r, vector follow) {
	// Resolve the x position
	if (room_width((*r)) * game.settings.zoom < WINDOW_WIDTH) {
		r->camera.x = room_width((*r)) / 2;
	} else {
		dimension min_x = WINDOW_WIDTH / 2 / game.settings.zoom;
		dimension max_x = room_width((*r)) - min_x;
		r->camera.x
			= (follow.x < min_x) ? min_x
			: (follow.x > max_x) ? max_x
			: follow.x;
	}

	// Resolve the y position
	if (room_height((*r)) * game.settings.zoom < WINDOW_HEIGHT) {
		r->camera.y = room_height((*r)) / 2;
	} else {
		dimension min_y = WINDOW_HEIGHT / 2 / game.settings.zoom;
		dimension max_y = room_height((*r)) - min_y;
		r->camera.y
			= (follow.y < min_y) ? min_y
			: (follow.y > max_y) ? max_y
			: follow.y;
	}
}
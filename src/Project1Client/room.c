#include "gamecontent.h"
#include <string.h>
#include <stdlib.h>
#include "naming.h"
#include "global.h"
#include "rem.h"

enum spawn_type { ST_OBJECT, ST_ENTITY };
struct spawn_details {
	enum spawn_type type;
	unsigned long id, x, y, row;
	void* custom_data;
};

static struct spawn_details init_spawn_0000[] = {
	{ ST_OBJECT, .id = 0, .x = 8, .y = 8, .row = 2  },
	{ ST_ENTITY, .id = ENTITY_DDUDE, .x = 32, .y = 32, .custom_data = NULL }
};
static struct spawn_details* game_room_spawnlist[ROOM_COUNT] = {
	init_spawn_0000
};

struct nge_room game_room[ROOM_COUNT] = {
	[ROOM_DEBUG] = {
		.horizontal_tile_count = 32,
		.vertical_tile_count   = 32,
		.object_count = 1,
		.entity_count = 1
	},
	[ROOM_BLANK] = { 0 }
};

int loadRoomAssets() {
	char tileset_name[TILESET_FORMAT_LEN];
	char tilemap_name[TILEMAP_FORMAT_LEN];
	for (unsigned long i = 0; i < ROOM_COUNT; i++) {
		game_room[i].id = i;

		sprintf(tileset_name, TILESET_FORMAT, i);
		sprintf(tilemap_name, TILEMAP_FORMAT, i);

		game_room[i].tile_set = remLoadImage(tileset_name);
		if (game_room[i].tile_set.id == 0) return 1;
		if (game_room[i].tile_map == NULL) {
			game_room[i].tile_map = remLoadResourceBytes(tilemap_name);
			if (game_room[i].tile_map == 0) return 2;
		}

		game_room[i].entities = remAlloc(sizeof(struct nge_entity) * game_room[i].entity_count);
		game_room[i].objects  = remAlloc(sizeof(struct nge_object) * game_room[i].object_count);
		for (unsigned long k = 0, j = 0; k < game_room[i].object_count + game_room[i].entity_count; k++) {
			struct spawn_details* subject = &(game_room_spawnlist[i][k]);
			if (subject->type == ST_OBJECT) {
				game_room[i].objects[j] = game_object[subject->id];
				game_room[i].objects[j].face.row.num = subject->row;
				game_room[i].objects[j].body.x = subject->x;
				game_room[i].objects[j].body.y = subject->y;
				j++;
			}
		}
		for (unsigned long k = 0, j = 0; k < game_room[i].object_count + game_room[i].entity_count; k++) {
			struct spawn_details* subject = &(game_room_spawnlist[i][k]);
			if (subject->type == ST_ENTITY) {
				game_room[i].entities[j] = game_entity[subject->id];
				game_room[i].entities[j].body.x = subject->x;
				game_room[i].entities[j].body.y = subject->y;
				game_room[i].entities[j].data = remAlloc(game_room[i].entities[j].data_len);
				memset(game_room[i].entities[j].data, 0, game_room[i].entities[j].data_len);
				if (game_room[i].entities[j].data != NULL && subject->custom_data != NULL) {
					memcpy(
						game_room[i].entities[j].data,
						subject->custom_data,
						game_room[i].entities[j].data_len
					);
				}
				j++;
			}
		}
	}

	return 0;
}
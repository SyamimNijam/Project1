#include "content/content_macro.h"
#include "entity.h"
#include "naming.h"
#include "rem.h"

DECL_UPDATE_FUNC(slime);
DECL_UPDATE_FUNC(ddude);

struct nge_entity game_entity[ENTITY_COUNT] = {
	[ENTITY_DDUDE] = {
		FULL_BODY(16, 32),
		.update = NAME_UPDATE_FUNC(ddude),
		.is_physical = true,
		.face = {
			.col = { .num = 0, .den = 1 },
			.row = { .num = 0, .den = 4 },
			.col_max = 1,
		}
	},
	[ENTITY_SLIME] = {
		FULL_BODY(16, 12),
		.face = NGE_STATIC_SPRITE,
		.update = NAME_UPDATE_FUNC(slime),
		.is_physical = true
	}
};

int loadEntityAssets() {
	char sprite_name[ENTITY_SPRITE_FORMAT_LEN];
	for (unsigned long i = 0; i < ENTITY_COUNT; i++) {
		game_entity[i].class_id = i;
		sprintf(sprite_name, ENTITY_SPRITE_FORMAT, i);
		game_entity[i].face.sheet = remLoadImage(sprite_name);
		if (game_entity[i].face.sheet.id == 0) return 1;
	}
	
	return 0;
}
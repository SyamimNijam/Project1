#include "content/content_macro.h"
#include "naming.h"
#include "object.h"
#include "rem.h"

struct nge_object game_object[OBJECT_COUNT] = {
	[OBJECT_CHAIR] = {
		FULL_BODY(16,16),
		.diagonal = 0,
		.is_physical = true,
		.face = {
			.col.den = 1,
			.col_max = 1,
			.row.den = 4,
		}
	},
};

int loadObjectAssets() {
	char sprite_name[OBJECT_SPRITE_FORMAT_LEN];
	for (unsigned long i = 0; i < OBJECT_COUNT; i++) {
		game_object[i].class_id = i;
		sprintf(sprite_name, OBJECT_SPRITE_FORMAT, i);
		game_object[i].face.sheet = remLoadImage(sprite_name);
		if (game_object[i].face.sheet.id == 0) return 1;
	}

	return 0;
}
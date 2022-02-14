#include "sprite.h"
#include "physics.h"
#include <string.h>
#include <stdlib.h>

void updateSprite(struct sprite* target, milliseconds delta) {
	if (target->col_max == 1) return;
	target->accumulated_time += delta;
	if (target->accumulated_time >= target->framelength_time) {
		target->col.num = (target->col.num + 1) % target->col_max;
		target->accumulated_time -= target->framelength_time;
	}
}

void loadStaticSprite(struct sprite* o) {
	image i = o->sheet;
	memset(o, 0, sizeof(struct sprite));
	o->sheet = i;
	o->col.den = 1;
	o->row.den = 1;
	o->col_max = 1;
	o->framelength_time = ~0;
}

void renderSprite(struct sprite* o, rectangle box, float d) {
	nbSetCropArea(
		(float)o->col.num / (float)o->col.den,
		(float)o->row.num / (float)o->row.den,
		1.0f / (float)o->col.den,
		1.0f / (float)o->row.den
	);
	nbSetTexture(o->sheet);
	nbDrawRect(expand_rectl(box), d);
}
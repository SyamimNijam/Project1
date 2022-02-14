#pragma once
#include <stdint.h>
#include "nb/nb.h"
#include "geometry.h"

#define NGE_STATIC_SPRITE {\
	.col = { .num = 0, .den = 1 },\
	.row = { .num = 0, .den = 1 },\
	.col_max = 1,\
}

// Sprite animation goes from left frame to right frame, set row index to change animation.
struct sprite {
	image sheet;
	ratio col, row;
	unsigned long col_max;
	milliseconds accumulated_time;
	milliseconds framelength_time;
};

// Sets the sprite of the pointer to a single frame non-moving sprite without image.
void loadStaticSprite(struct sprite* s);
// Updates the sprite's values based on how much time has passed since the last update.
void updateSprite(struct sprite* s, milliseconds timedelta_ms);
// Render the sprite to destination on a certain depth.
void renderSprite(struct sprite* s, rectangle destination, float depth);
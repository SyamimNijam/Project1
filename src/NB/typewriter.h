#pragma once
#include "graphics.h"
#include <stdarg.h>

#define N_ASCII_CHARS 128

typedef image_id ascii_fontset[N_ASCII_CHARS];

// Render formatted text passed through vsnprintf starting from first_x and first_y.
// Returns value given by vsnprintf.
int nbDrawTextf(
	pixels first_x,
	pixels first_y,
	pixels char_w,
	pixels char_h,
	ascii_fontset,
	float depth,
	const char* format,
	...
);
// Render formatted text passed through vsnprintf starting from first_x and first_y.
// Returns value given by vsnprintf.
int nbDrawTextvf(
	pixels first_x,
	pixels first_y,
	pixels char_w,
	pixels char_h,
	ascii_fontset,
	float depth,
	const char* format,
	va_list
);
// Render text starting from first_x and first_y.
void nbDrawText(
	pixels first_x,
	pixels first_y,
	pixels char_w,
	pixels char_h,
	ascii_fontset,
	float depth,
	const char* text
);
// Loads a fontset from image data.
void nbGenerateFontset(ascii_fontset,
	image_data data, size_t data_len, pixels width);
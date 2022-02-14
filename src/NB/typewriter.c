#include "typewriter.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void nbGenerateFontset(ascii_fontset out,
	image_data data, size_t data_len, pixels width
) {
	pixels font_pwidth = width;
	pixels font_pheight = (pixels)(((data_len / 4) / width) / N_ASCII_CHARS);
	size_t symbol_len = data_len / N_ASCII_CHARS;

	for (unsigned long i = 0; i < N_ASCII_CHARS; i++) {
		out[i] = nbLoadImageFromBytes(
			&data[symbol_len * i],
			font_pheight, font_pwidth
		).id;
	}
}

int nbDrawTextf(pixels x, pixels y, pixels w, pixels h,
	ascii_fontset ft, float depth, const char* format, ...) {
	va_list s;
	va_start(s, format);
	int retval = nbDrawTextvf(x, y, w, h, ft, depth, format, s);
	va_end(s);
	return retval;
}

int nbDrawTextvf(pixels x, pixels y, pixels w, pixels h,
	ascii_fontset ft, float depth, const char* format, va_list v) {
	char buffer[4096] = { 0 };
	int retval = vsnprintf(buffer, sizeof(buffer), format, v);
	nbDrawText(x, y, w, h, ft, depth, (const char*)buffer);
	return retval;
}

void nbDrawText(pixels x, pixels y, pixels w, pixels h,
	ascii_fontset ft, float depth, const char* c_str) {
	pixels original_x = x;
	nbSetCropArea(0.0f, 0.0f, 1.0f, 1.0f);
	for (unsigned long i = 0; i < strlen(c_str); i++) {
		char symbol = (c_str[i] < 0) ? N_ASCII_CHARS-1 : c_str[i];
		switch (symbol) {
		case '\t':
			x += w * 4;
			break;
		case '\n':
			x = original_x;
			y -= h;
			break;
		default:
			nbSetTextureId(ft[(unsigned long)symbol]);
			nbDrawRect(x, y, w, h, depth);
			x += w;
		}
	}
}
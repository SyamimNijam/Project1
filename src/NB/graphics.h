#pragma once
#include <stdint.h>

typedef const unsigned char* image_data;
typedef uint32_t image_id;
typedef uint8_t byte;
typedef long pixels;
typedef struct _image {
	image_id id;
	pixels height, width;
} image;

// Loads an image for rendering from pixel data
// (RGBA, 32bpp, left to right scanlines, render bottom row first to top)
image nbLoadImageFromBytes(image_data, pixels height, pixels width);

// Sets the scale factor for all draw functions
void nbSetScale(float scale);
// Get scale factor for all draw functions
float nbGetScale();
// The x and y positions will be added offset by the translate before nbDrawRect
void nbSetTranslate(pixels x, pixels y);
// Get the x offset
pixels nbGetTranslateX();
// Get the y offset
pixels nbGetTranslateY();

// Draws a rectangle with currently set texture
void nbDrawRect(pixels x, pixels y, pixels w, pixels h, float depth);
// Sets the portion of the image to render
void nbSetCropArea(float x, float y, float w, float h);
// Sets the tint or color if blank
void nbSetTint(byte r, byte g, byte b, byte a);
// Sets the color used when clearing the screen
void nbClearColor(byte r, byte g, byte b);
// Sets the texture to be rendered when calling nbDrawRect
void nbSetTextureId(image_id i);
// Sets the texture to be rendered when calling nbDrawRect
void nbSetTexture(image texture);
// Fills the screen with the color set by nbClearColor
void nbClear();
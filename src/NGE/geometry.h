#pragma once
#include <stdint.h>

typedef long dimension;
typedef struct {
	long num; long den;
} ratio;

typedef struct vector {
	dimension x, y;
} vector;

typedef struct line {
	ratio slope;
	dimension c;
} line;

typedef struct rectangle {
	dimension x, y;
	dimension width, height;
} rectangle;

typedef struct region {
	dimension left, right, top, bottom;
} region;

// Calculates the line between a region's diagonals.
line getRegionDiagonal(region rg);

// Math macros

#define rect_position(r) (vector){ r.x, r.y }

#define expand_rect(r) r.x, r.y, r.width, r.height
#define expand_rect_cast(r, type) (type)r.x, (type)r.y, (type)r.width, (type)r.height
#define expand_rectl(r) expand_rect_cast(r, long)
#define expand_rectf(r) expand_rect_cast(r, float)

#define line_y(l, x) (dimension)(line_m(l)*x+l.c)
#define line_x(l, y) (dimension)((y-l.c)/line_m(l))
#define line_m(l) (l.slope.num / l.slope.den)

#define region_height(rg) (rg.top-rg.bottom)
#define region_width(rg) (rg.right-rg.left)
#define region_x(rg) ((rg.right+rg.left)/2)
#define region_y(rg) ((rg.top+rg.bottom)/2)
#define rect_left(rt) (rt.x-rt.width/2)
#define rect_right(rt) (rt.x+rt.width/2)
#define rect_top(rt) (rt.y+rt.height/2)
#define rect_bottom(rt) (rt.y-rt.height/2)

#define region_center(rg) (vector){.x=region_x(rg),.y=region_y(rg)}
#define region_global(rg, offset) (region){.left=rg.left+offset.x,.right=rg.right+offset.x,.top=rg.top+offset.y,.bottom=rg.bottom+offset.y}

#define region_to_rect(rg) (rectangle){.x=region_x(rg),.y=region_y(rg),.width=region_width(rg),.height=region_height(rg)}
#define rect_to_region(rt) (region){.left=rect_left(rt),.right=rect_right(rt),.top=rect_top(rt),.bottom=rect_bottom(rt)}
#include "geometry.h"

line getRegionDiagonal(region rg) {
	line ret = { 0 };
	ret.slope.num = region_height(rg);
	ret.slope.den = region_width(rg);
	ret.c = (dimension)(region_y(rg) - line_m(ret) * region_x(rg));
	return ret;
}
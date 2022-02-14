#include "physics.h"

void pushBackEntityFromDiagonal135(dimension c, struct nge_entity* entity) {
	vector origin = region_center((region_global(entity->collision, rect_position(entity->body))));
	if (origin.y > -origin.x + c) {
		// above
		dimension top_intersect_y = -(entity->collision.left + entity->body.x) + c;
		dimension center_intersect_y = (top_intersect_y + entity->collision.bottom + entity->body.y) / 2;
		entity->body.y = center_intersect_y - entity->collision.bottom;
	} else {
		// below
		dimension bottom_intersect_y = -(entity->collision.right + entity->body.x) + c;
		dimension center_intersect_y = (bottom_intersect_y + entity->collision.top + entity->body.y) / 2;
		entity->body.y = center_intersect_y - entity->collision.top;
	}
}

int isDiagonallyColliding135(dimension c, region region_b) {
	if ((region_b.top > -region_b.right + c) && (region_b.bottom < -region_b.left + c))
		return 1;
	return 0;
}

int isDiagonallyColliding45(dimension c, region region_b) {
	if ((region_b.top > region_b.left + c) && (region_b.bottom < region_b.right + c))
		return 1;
	return 0;
}

void pushBackEntityFromDiagonal45(dimension c, struct nge_entity* entity) {
	vector origin = region_center((region_global(entity->collision, rect_position(entity->body))));
	if (origin.y > origin.x + c) {
		// above
		dimension top_intersect_y = entity->collision.right + entity->body.x + c;
		dimension center_intersect_y = (top_intersect_y + entity->collision.bottom + entity->body.y) / 2;
		entity->body.y = center_intersect_y - entity->collision.bottom;
	} else {
		// below
		dimension bottom_intersect_y = entity->collision.left + entity->body.x + c;
		dimension center_intersect_y = (bottom_intersect_y + entity->collision.top + entity->body.y) / 2;
		entity->body.y = center_intersect_y - entity->collision.top;
	}
}

void pushBackEntityFromDiagonal(line diag, struct nge_entity* entity) {
	/*
	//region ent_collision = region_global(entity->collision, entity->body.position);
	vector corner;
	if (entity->body.y < entity->body.x * diag.m + diag.c) {
		// under
		corner.y = entity->collision.top;
		if (diag.m >= 0) {
			// top left corner
			corner.x = entity->collision.left;
		} else {
			// top right corner
			corner.x = entity->collision.right;
		}
	} else {
		// over
		corner.y = entity->collision.bottom;
		if (diag.m >= 0) {
			// bottom right corner
			corner.x = entity->collision.right;
		} else {
			// bottom left corner
			corner.x = entity->collision.left;
		}
	}
	line anti_line = {
		.m = -1 / diag.m,
		.c = (corner.y+entity->body.y) - anti_line.m * (corner.x+entity->body.x)
	};
	// line.m*x+line.c = anti_line.m*x+anti_line.c
	// line.m*x - anti_line.m*x = anti_line.c - line.c
	// (line.m - anti_line.m) * x = anti_line.c - line.c
	// x = (anti_line.c - line.c) / (line.m - anti_line.m)
	dimension intersect_x = (anti_line.c - diag.c) / (diag.m - anti_line.m) + 1;
	entity->body.x = intersect_x-corner.x;
	entity->body.y = line_y(anti_line, intersect_x)-corner.y;
	*/
	printf("Push back entity from diagonal not implemented yet.\r\n");
}

int isDiagonallyColliding(line diag, region region_b) {
	// f(x) = mx + c
	// f^-1(x) = (x-c)/m
	vector b_center = region_center(region_b);
	if (diag.slope.den * diag.slope.num >= 0) {
		// Positive slope ( / )
		// Check if care about top left or bottom right corners.
		return (b_center.y > line_y(diag, b_center.x)) ?
			// We care about the bottom right corner.
			(region_b.bottom < line_y(diag, region_b.right))
			:
			// We care about the top left corner.
			(region_b.top > line_y(diag, region_b.left));
	} else {
		// Negative slope ( \ )
		// Check if we care about top right or bottom left corners.
		return (b_center.y > line_y(diag, b_center.x)) ?
			// We care about bottom left corner.
			(region_b.bottom < line_y(diag, region_b.left))
			:
			// We care about top right corner.
			(region_b.top > line_y(diag, region_b.right));
	}
}

int isColliding(region region_a, region region_b) {
	if (region_a.top < region_b.bottom ||
			region_a.bottom > region_b.top ||
				region_a.left > region_b.right ||
					region_a.right < region_b.left)
						return 0;
	else
		return 1;
}

void pushBackEntityFromRegion(region static_region, struct nge_entity* entity) {
	vector static_region_center = region_center(static_region);
	region entity_global_region = region_global(entity->collision, rect_position(entity->body));
	vector entity_region_center = region_center(entity_global_region);
	float x = (float)entity_region_center.x - (float)static_region_center.x;
	float y = (float)entity_region_center.y - (float)static_region_center.y;
	float xpy = (float)region_width(static_region) / (float)region_height(static_region);
	if (x > -1 * xpy * y) {
		// Entity is above or to the right of the object
		x = (float)entity_global_region.left - (float)static_region_center.x;
		y = (float)entity_global_region.bottom - (float)static_region_center.y;
		if (x >= xpy * y)
			entity->body.x = static_region.right - entity->collision.left;
		else
			entity->body.y = static_region.top - entity->collision.bottom;
	} else {
		// Entity is below or to the left of the object
		x = (float)entity_global_region.right - (float)static_region_center.x;
		y = (float)entity_global_region.top - (float)static_region_center.y;
		if (x < xpy * y)
			entity->body.x = static_region.left - entity->collision.right;
		else
			entity->body.y = static_region.bottom - entity->collision.top;
	}
}
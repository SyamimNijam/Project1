#pragma once
#include "geometry.h"
#include "game.h"

// Assuming the entity is colliding with region, it pushes the entity out of the region.
void pushBackEntityFromRegion(region static_region, struct nge_entity* entity);
// Checks if two regions are colliding.
int isColliding(region region_a, region region_b);

// Assuming the entity is crossing the line, it pushes the entity out of the line with the least effort.
void pushBackEntityFromDiagonal(line diagonal, struct nge_entity* entity);
// Checks if region_b is crossing the diagonal line.
int isDiagonallyColliding(line diagonal, region region_b);

// Checks if region_b is crossing the line y = x+c.
int isDiagonallyColliding45(dimension c, region region_b);
// Checks if region_b is crossing the line y = c-x.
int isDiagonallyColliding135(dimension c, region region_b);
// Assuming the entity is crossing the line, it pushes the entity out of the line y=x+c.
void pushBackEntityFromDiagonal45(dimension c, struct nge_entity* entity);
// Assuming the entity is crossing the line, it pushes the entity out of the line y=c-x.
void pushBackEntityFromDiagonal135(dimension c, struct nge_entity* entity);
#include "nb/nb.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "game.h"
#include "physics.h"

#define COLLECT_PER_UNIT 1000000
#define collect(result, collect) { result += collect/COLLECT_PER_UNIT; collect %= COLLECT_PER_UNIT; }
#define inc_collect(result, collect, increase) { collect += increase; collect(result, collect); }

void ngeUpdateRoom(struct nge_room* r, milliseconds delta) {
	// Update sprites
	for (unsigned long i = 0; (i < r->object_count) || (i < r->entity_count); i++) {
		if (i < r->entity_count)
			updateSprite(&r->entities[i].face, delta);
		if (i < r->object_count)
			updateSprite(&r->objects[i].face, delta);
	}
	// Update entities
	for (unsigned long i = 0; i < r->entity_count; i++) {
		struct nge_entity* ent = &r->entities[i];

		// Collect system for smooth moving
		long previous_x = ent->body.x;
		if (ent->velocity.x != 0) {
			ent->position_collect.x += (long)(
					(milliseconds)ent->velocity.x
					* COLLECT_PER_UNIT
					* (delta / 1000.0)
				);
			collect(ent->body.x, ent->position_collect.x);
		} else
			ent->position_collect.x = 0;
		if (ent->velocity.y != 0) {
			ent->position_collect.y += (long)(
				(milliseconds)ent->velocity.y
				* COLLECT_PER_UNIT
				* (delta / 1000.0)
				);
			if (previous_x != ent->body.x) {
				ent->position_collect.y
					= (ent->position_collect.y > 0) ?
						COLLECT_PER_UNIT : -COLLECT_PER_UNIT;
			}
			collect(ent->body.y, ent->position_collect.y);
		} else
			ent->position_collect.y = 0;

		region ent_collision = region_global(ent->collision, rect_position(ent->body));
		// Prevent entities from colliding with objects
		if (ent->is_physical) {
			for (unsigned long oi = 0; oi < r->object_count; oi++) {
				struct nge_object* obj = &r->objects[oi];
				region obj_collision = region_global(obj->collision, rect_position(obj->body));
				if (r->objects[oi].is_physical && isColliding(ent_collision, obj_collision)) {
					if (obj->diagonal) {
						if (obj->diagonal > 0) {
							dimension obj_c = obj_collision.bottom-obj_collision.left;
							if (isDiagonallyColliding45(obj_c, ent_collision)) {
								pushBackEntityFromDiagonal45(obj_c, ent);
								ent->is_touching = 1;
							}
						} else {
							dimension obj_c = obj_collision.bottom + obj_collision.right;
							if (isDiagonallyColliding135(obj_c, ent_collision)) {
								pushBackEntityFromDiagonal135(obj_c, ent);
								ent->is_touching = 1;
							}
						}
					} else {
						pushBackEntityFromRegion(obj_collision, ent);
						ent->is_touching = 1;
					}
				}
			}
		}

		// Update if available
		if (ent->update)
			ent->update(r, ent, delta);

		// Compute collisions between entities
		if (ent->react_flags) {
			for (unsigned long ei = 0; ei < r->entity_count; ei++) {
				if (ei == i) continue;
				struct nge_entity* other_ent = &r->entities[ei];
				if (ent->react_flags & other_ent->collision_flags) {
					region other_region = region_global(
						other_ent->collision,
						rect_position(other_ent->body)
					);
					if (isColliding(ent_collision, other_region)) {
						ent->collide(r, ent, other_ent);
					}
				}
			}
		}
	}
}

void ngeRenderRoom(struct nge_room* rm, unsigned long scale) {
	nbSetTranslate(-rm->camera.x, -rm->camera.y);
	nbSetScale((float)scale);
	// Render tiles
	nbSetTexture(rm->tile_set);
	float tile_width_texel = (float)rm->tile_set.height / (float)rm->tile_set.width;
	for (unsigned long tile_y = 0; tile_y < rm->vertical_tile_count; tile_y++) {
		for (unsigned long tile_x = 0; tile_x < rm->horizontal_tile_count; tile_x++) {
			unsigned char current_tile
				= rm->tile_map[tile_y * rm->horizontal_tile_count + tile_x];
			nbSetCropArea(tile_width_texel * current_tile, 0.0f, tile_width_texel, 1.0f);
			nbDrawRect(
				tile_x * (long)rm->tile_set.height + (long)rm->tile_set.height/2,
				tile_y * (long)rm->tile_set.height + (long)rm->tile_set.height/2,
				(long)rm->tile_set.height,
				(long)rm->tile_set.height,
				1.0f
			);
		}
	}
	// Render objects before entities in the same for loop
	// Note: If you have a better way of doing this, do it
	for (unsigned long i = 0; i < rm->object_count + rm->entity_count; i++) {
		struct sprite* sp;
		rectangle body;
		
		if (i < rm->object_count) {
			sp = &rm->objects[i].face;
			body = rm->objects[i].body;
		} else {
			sp = &rm->entities[i - rm->object_count].face;
			body = rm->entities[i - rm->object_count].body;
		}
		
		float world_height = (float)rm->vertical_tile_count * rm->tile_set.height;
		float depth = (float)rect_bottom(body)/(world_height/2.0f) - 1.0f;
		renderSprite(sp, body, depth);

		if (i >= rm->object_count) {
			struct nge_entity* self = &rm->entities[i - rm->object_count];
			if (self->render != NULL) {
				self->render(rm, self);
				nbSetTranslate(-rm->camera.x, -rm->camera.y);
				nbSetScale((float)scale);
			}
		}
	}
}

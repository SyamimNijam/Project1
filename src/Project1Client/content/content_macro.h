#pragma once

#define true 1
#define false 0

#define NAME_UPDATE_FUNC(x) ent_update_ ## x
#define DECL_UPDATE_FUNC(x)\
 void NAME_UPDATE_FUNC(x) (struct nge_room* r,self_entity x,milliseconds delta)

#define FULL_BODY(w, h)\
		.body = { .width = w, .height = h },\
		.collision = { .left = -(w)/2, .right = (w)/2, .top = (h)/2, .bottom = -(h)/2}

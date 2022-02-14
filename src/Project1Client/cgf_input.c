#include "gameinput.h"
#include "gamestate.h"
#include "cgf.h"

int cgf_input_enabled = 0;
void cgfEnableInput() { cgf_input_enabled = 1; }
void cgfDisableInput() { cgf_input_enabled = 0; }

bool cgfIsKeyTapped(enum nb_key key) {
	static bool key_map[NB_KEY_MAX] = { 0 };
	if (nbGetKeyState(key)) {
		if (key_map[key]) {
			return false;
		} else {
			key_map[key] = 1;
			return true;
		}
	} else {
		key_map[key] = 0;
		return false;
	}
}

vector cgfGetInputVelocity(dimension max) {
	vector velocity;
	velocity.x = INPUT_RIGHT ? max : INPUT_LEFT ? -max : 0;
	velocity.y = INPUT_UP ? max : INPUT_DOWN ? -max : 0;
	return velocity;
}

enum direction cgfGetVectorDirection(vector v) {
	if (v.y > 0) return DIRECTION_N;
	if (v.y < 0) return DIRECTION_S;
	if (v.x > 0) return DIRECTION_E;
	if (v.x < 0) return DIRECTION_W;
	return DIRECTION_DEFAULT;
}

void cgfControlEntity(struct nge_entity* r, dimension spd) {
	if (cgf_input_enabled) {
		r->velocity = cgfGetInputVelocity(spd);
		if (r->velocity.y || r->velocity.x)
			r->face.row.num = cgfGetVectorDirection(r->velocity);
	}
}
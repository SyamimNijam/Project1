#include "gamecontent.h"
#include "gameinput.h"
#include <string.h>
#include "global.h"
#include "gamestate.h"
#include "cgf.h"
#include "rem.h"

long cgfGetDirectionalRow(long row, enum direction dir) {
	row /= 4;
	return row * 4 + dir;
}

vector cgfGetMouseRoomPos(vector camera, unsigned long scale) {
	vector position;
	nbGetMousePosition(&position.x, &position.y);
	position.x = position.x / (long)scale + camera.x;
	position.y = position.y / (long)scale + camera.y;
	return position;
}

vector cgfWorldToScreen(vector camera, unsigned long scale, vector wld) {
	return (vector){
		.x = (wld.x - camera.x) * scale,
		.y = (wld.y - camera.y) * scale,
	};
}
vector cgfScreenToWorld(vector camera, unsigned long scale, vector scr) {
	return (vector) {
		.x = scr.x / scale - camera.x,
		.y = scr.y / scale - camera.y,
	};
}
#include "gamecontent.h"
#include "rem.h"

#define check(x) { ret = x; if (ret) { return ret; } }

struct game_state game = { 0 };

int loadGameAssets() {
	int ret = 0;
	check(loadObjectAssets());
	check(loadEntityAssets());
	check(loadRoomAssets());
	
	return 0;
}

void releaseGameAssets() {

	remFreeAll();
}
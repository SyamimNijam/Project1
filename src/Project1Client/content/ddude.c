#include "content_macro.h"
#include "../gamecontent.h"
#include "../global.h"
#include <NGE/nge.h>
#include <stdio.h>
#include "../gamestate.h"
#include "../cgf.h"

DECL_UPDATE_FUNC(ddude) {
	// Static variables
	static dimension speed = 64;
	// LSHIFT to run
	speed = (nbGetKeyState(NB_KEY_LSHIFT) || nbGetKeyState(NB_KEY_RSHIFT)) ? 128 : 64;
	cgfControlEntity(ddude, speed);
	cgfMoveCamera(r, rect_position(ddude->body));
}
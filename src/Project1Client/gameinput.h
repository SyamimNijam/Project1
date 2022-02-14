#pragma once
#include <NB/nb.h>
 
#define keyDown(x) nbGetKeyState(x)
#define INPUT_UP (keyDown(NB_KEY_W) || keyDown(NB_KEY_UP))
#define INPUT_RIGHT (keyDown(NB_KEY_D) || keyDown(NB_KEY_RIGHT))
#define INPUT_DOWN (keyDown(NB_KEY_S) || keyDown(NB_KEY_DOWN))
#define INPUT_LEFT (keyDown(NB_KEY_A) || keyDown(NB_KEY_LEFT))
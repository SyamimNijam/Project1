#pragma once
#include <stdint.h>
#include "typewriter.h"
#include "graphics.h"
#include "input.h"

typedef double milliseconds;

// Initializes nb and sets up a window.
int nbInit(const wchar_t* title, unsigned long width, unsigned long height);
// Get the number of milliseconds since nb was initialized.
milliseconds nbGetTicks();
// Swaps the main window's buffer.
void nbSwapBuffers();
// Non-zero if window is still up.
int nbWindowIsUp();
// Polls the window for events.
void nbPollEvent();
// Releases all resources used by nbml and closes the window.
void nbQuit();
#include <NGE/game.h>
#include <stdlib.h>
#include <string.h>
#include <NB/nb.h>
#include <stdio.h>
#include <math.h>

#include "gamecontent.h"
#include "global.h"
#include "cgf.h"
#include "gui.h"

#define chkrr(x) { retval = x;\
 if (retval) { fprintf(stderr, "Error code %i from " #x "\r\n", retval);\
 goto exit; } }

const struct settings default_settings = {
	.zoom = 4
};

#define POC_SETTINGS_FILE "settings"

// Functions

static int  fileExists(const char* uri);
static int  initializeWindow();
static void readSettings();

// Main

int main(int argc, char* argv[]) {
	printf("Here we go...\r\n\r\n");



	// Initialization
	int retval = 0;
	chkrr(initializeWindow());
	readSettings();
	chkrr(loadGameAssets());
	chkrr(loadGUIAssets());
	/* ------- TEMPORARY -------- */
	game.current_room_id = ROOM_DEBUG;
	cgfEnableInput();
	/* -------------------------- */

	// Main game loop
	milliseconds last_render = nbGetTicks();
	milliseconds last_update = nbGetTicks();

	while (1) {
		// Quit when window quits
		nbPollEvent();
		if (!nbWindowIsUp())
			break;

		// Update cycle
		milliseconds now = nbGetTicks();
		if (!game.is_paused)
			ngeUpdateRoom(&CURRENT_ROOM, now - last_update);
		last_update = now;

		// Render cycle
		if (nbGetTicks() > last_render + 1000.0 / FPS) {
			nbClear();
			ngeRenderRoom(&CURRENT_ROOM, game.settings.zoom);
			renderGUI();
	
			nbSwapBuffers();
			last_render = nbGetTicks();
		}
	}
	
exit:

	// Release resources
	releaseGameAssets();
	nbQuit();

	// Exit
	printf("Project1Client finished cleanly.\r\n");
	return retval;
}

static int fileExists(const char* uri) {
	FILE* f = fopen(uri, "r");
	if (f) fclose(f);
	return (f) ? 1 : 0;
}

static int initializeWindow() {
	int retval = 0;
	retval = nbInit(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (retval != 0) return retval;
	nbClearColor(0x00, 0x00, 0x00);
	return retval;
}

static void readSettings() {
	FILE* settings_file = NULL;
	if (fileExists(POC_SETTINGS_FILE)) {
		settings_file = fopen(POC_SETTINGS_FILE, "rb");
		fread(&game.settings, sizeof(game.settings), 1, settings_file);
	} else {
		game.settings = default_settings;
	}
	if (settings_file)
		fclose(settings_file);
}
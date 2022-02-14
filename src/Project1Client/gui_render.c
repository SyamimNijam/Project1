#include <NGE/nge.h>
#include <string.h>
#include <NB/nb.h>

#include "entity.h"
#include "object.h"
#include "room.h"

#include "gamestate.h"
#include "global.h"
#include "rem.h"
#include "gui.h"

ascii_fontset   gui_font;

extern void gui_keyhandler(enum nb_key);
extern void gui_texthandler(char);

int loadGUIAssets() {
	if (remLoadFont("_font", gui_font)) return 4;
	if (nbRegisterTextEventHandler(gui_texthandler)) return 5;
	if (nbRegisterKeyEventHandler(gui_keyhandler)) return 6;

	return 0;
}

extern struct gui_button* gui_button_stack[GUI_BUTTON_STACK_LEN];
extern long gui_button_sp;

extern struct gui_textbox* gui_textbox;

static void renderButton(struct gui_button* r) {
	nbDrawText(
		(pixels)r->x, (pixels)r->y,
		GUI_FONT_W, GUI_FONT_H,
		gui_font, GUI_DEPTH,
		r->text
	);
}

static void renderButtonMenu(struct gui_button* first) {
	if (first == NULL) return;
	struct gui_button* next;
	next = first;
	while (next != NULL) {
		renderButton(next);
		next = next->next_button;
		if (next == first) return;
	}
	next = first->prev_button;
	while (next != NULL) {
		renderButton(next);
		next = next->prev_button;
		if (next == first) return;
	}
}

void renderGUI() {
	nbSetScale(1.0f);
	nbSetTranslate(0, 0);

	for (long i = 0; i <= gui_button_sp; i++) {
		renderButtonMenu(gui_button_stack[i]);
	}
	static milliseconds last = 0;
	static int toggle = 0;
	milliseconds now = nbGetTicks();
	if (now - last >= 400) {
		last = nbGetTicks();
		toggle ^= 1;
	}
	if (gui_textbox != NULL) {
		char* pos = &gui_textbox->buffer_ptr[
			gui_textbox->buffer_pos
			- (gui_textbox->buffer_pos == gui_textbox->buffer_len - 1)];
		char original = *pos;
		if (toggle) *pos = '_';
		nbDrawText(
			gui_textbox->x, gui_textbox->y,
			GUI_FONT_W, GUI_FONT_H,
			gui_font, GUI_DEPTH,
			gui_textbox->buffer_ptr
		);
		*pos = original;
	}
}
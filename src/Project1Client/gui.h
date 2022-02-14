#pragma once
#include "global.h"
#include <NB/nb.h>

#define GUI_BUTTON_STACK_LEN 32

#define GUI_FONT_H (16*2)
#define GUI_FONT_W (8 *2)
#define GUI_DEPTH -1.0f

extern ascii_fontset gui_font;

// GUI System

struct gui_button {
	long x, y;

	struct gui_button* prev_button;
	struct gui_button* next_button;

	const char* text;
	void (*handle_event)(struct gui_button*);
};
struct gui_textbox {
	long x, y;

	char*  buffer_ptr;
	size_t buffer_len;
	size_t buffer_pos;
};

// Sets the current textbox for user to input text in
// Set NULL to disable
void guiSetTextbox(struct gui_textbox*);
// Pushes a button pointer on the stack, used for buttons that expand menus
// Returns 1 if stack is full
int guiPushButtonStack(struct gui_button*);
// Pops a button pointer from the stack which was pushed by guiPushButtonStack
// Returns NULL if stack is empty
struct gui_button* guiPopButtonStack();

// Rendering

// Loads assets used by GUI
int loadGUIAssets();
// Renders GUI and SFX
void renderGUI();
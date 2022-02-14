#include <string.h>
#include "cgf.h"
#include "gui.h"

struct gui_button* gui_button_stack[GUI_BUTTON_STACK_LEN] = { 0 };
long gui_button_sp = -1;

struct gui_textbox* gui_textbox = NULL;

void guiSetTextbox(struct gui_textbox* tb) {
	gui_textbox = tb;
	if (tb == NULL && gui_button_sp < 0)
		cgfEnableInput();
	else
		cgfDisableInput();
}

void gui_texthandler(char c) {
	if (gui_textbox != NULL) {
		if (c == '\b') {
			if (gui_textbox->buffer_pos > 0) {
				memmove(
					&gui_textbox->buffer_ptr[gui_textbox->buffer_pos - 1],
					&gui_textbox->buffer_ptr[gui_textbox->buffer_pos + 0],
					gui_textbox->buffer_len - gui_textbox->buffer_pos
				);
				gui_textbox->buffer_pos--;
			}
		} else if(gui_textbox->buffer_pos + 1 < gui_textbox->buffer_len) {
			if (c == '\r') c = '\n';
			gui_textbox->buffer_ptr[gui_textbox->buffer_pos++] = c;
		}
	}
}

void gui_keyhandler(enum nb_key e) {
	struct gui_textbox* const tb = gui_textbox;
	if (tb != NULL) {
		// Handle textbox
		size_t text_len = strlen(tb->buffer_ptr);
		if (text_len > 0) {
			switch (e) {
				case NB_KEY_RIGHT:
					if (tb->buffer_pos < text_len)
						tb->buffer_pos++;
					break;
				case NB_KEY_LEFT:
					if (tb->buffer_pos > 0)
						tb->buffer_pos--;
					break;
				default:
					break;
			}
		}
	} else if (gui_button_sp >= 0) {
		// Handle buttons
		struct gui_button* b = gui_button_stack[gui_button_sp];
		switch (e) {
			case NB_KEY_ESCAPE:
				guiPopButtonStack();
				break;
			case NB_KEY_RETURN:
				b->handle_event(b);
				break;
			case NB_KEY_RIGHT:
			case NB_KEY_DOWN:
				if (b->next_button)
					gui_button_stack[gui_button_sp] = b->next_button;
				break;
			case NB_KEY_LEFT:
			case NB_KEY_UP:
				if (b->prev_button)
					gui_button_stack[gui_button_sp] = b->prev_button;
				break;
			default:
				break;
		}
	}
}

int guiPushButtonStack(struct gui_button* b) {
	if (gui_button_sp + 1 == GUI_BUTTON_STACK_LEN)
		return 1;
	gui_button_stack[++gui_button_sp] = b;
	cgfDisableInput();
	return 0;
}

struct gui_button* guiPopButtonStack() {
	if (gui_button_sp < 0) return NULL;
	if (gui_button_sp == 0) cgfEnableInput();
	return gui_button_stack[gui_button_sp--];
}
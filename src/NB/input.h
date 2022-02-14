#pragma once

enum nb_key;

typedef void (*key_event_handler)(enum nb_key);
typedef void (*text_event_handler)(char);

// Register a function that receives a char. Returns 0 on success.
int nbRegisterTextEventHandler(text_event_handler);
// Register a function that receives key events. Returns 0 on success.
int nbRegisterKeyEventHandler(key_event_handler);
// Non-zero if key is currently being pressed.
int nbGetKeyState(enum nb_key k);
// Gets position of mouse in pixels relative to center of window.
void nbGetMousePosition(long* x, long* y);

enum nb_key {
	// Mouse
	NB_KEY_LMOUSE = 0x01,
	NB_KEY_RMOUSE = 0x02,
	NB_KEY_MMOUSE = 0x04,

	// Special
	NB_KEY_RETURN = 0x0D,
	NB_KEY_BACKSPACE = 0x08,
	NB_KEY_ESCAPE = 0x1B,
	NB_KEY_SPACE = 0x20,
	NB_KEY_TAB = 0x09,

	// Directional
	NB_KEY_LEFT = 0x25, NB_KEY_UP, NB_KEY_RIGHT, NB_KEY_DOWN,

	// Top row numbers
	NB_KEY_0 = 0x30,
	NB_KEY_1, NB_KEY_2, NB_KEY_3,
	NB_KEY_4, NB_KEY_5, NB_KEY_6,
	NB_KEY_7, NB_KEY_8, NB_KEY_9,

	// Alphabet
	NB_KEY_A = 0x41,
	NB_KEY_B, NB_KEY_C, NB_KEY_D,
	NB_KEY_E, NB_KEY_F, NB_KEY_G, NB_KEY_H,
	NB_KEY_I, NB_KEY_J, NB_KEY_K, NB_KEY_L,
	NB_KEY_M, NB_KEY_N, NB_KEY_O, NB_KEY_P,
	NB_KEY_Q, NB_KEY_R, NB_KEY_S, NB_KEY_T,
	NB_KEY_U, NB_KEY_V, NB_KEY_W, NB_KEY_X,
	NB_KEY_Y, NB_KEY_Z,

	// Misc.
	NB_KEY_SEMICOLON = 0xBA,
	NB_KEY_EQUALS,
	NB_KEY_COMMA,
	NB_KEY_MINUS,
	NB_KEY_PERIOD,
	NB_KEY_SLASH, // Forward slash
	NB_KEY_TILDE,
	NB_KEY_LBRACE = 0xDB,
	NB_KEY_BACKSLASH,
	NB_KEY_RBRACE,
	NB_KEY_QUOTE,

	// Numpad
	NB_KEY_NUMPAD0 = 0x60,
	NB_KEY_NUMPAD1, NB_KEY_NUMPAD2, NB_KEY_NUMPAD3,
	NB_KEY_NUMPAD4, NB_KEY_NUMPAD5, NB_KEY_NUMPAD6,
	NB_KEY_NUMPAD7, NB_KEY_NUMPAD8, NB_KEY_NUMPAD9,

	// Function keys
	NB_KEY_F1 = 0x70,
	NB_KEY_F2 , NB_KEY_F3 , NB_KEY_F4 ,
	NB_KEY_F5 , NB_KEY_F6 , NB_KEY_F7 , NB_KEY_F8 ,
	NB_KEY_F9 , NB_KEY_F10, NB_KEY_F11, NB_KEY_F12,
	NB_KEY_F13, NB_KEY_F14, NB_KEY_F15, NB_KEY_F16,
	NB_KEY_F17, NB_KEY_F18, NB_KEY_F19, NB_KEY_F20,
	NB_KEY_F21, NB_KEY_F22, NB_KEY_F23, NB_KEY_F24,

	// Modifier
	NB_KEY_LSHIFT = 0xA0, NB_KEY_RSHIFT,
	NB_KEY_LCTRL, NB_KEY_RCTRL,

	// Not keys
	NB_KEY_MAX,
	NB_KEY_COUNT = 82
};
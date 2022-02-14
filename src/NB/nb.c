#include "nbml_internal.h"
#include "graphics.h"
#include "input.h"
#include <time.h>
#include "nb.h"

int window_state = 0;
int nbWindowIsUp() { return window_state; }

static text_event_handler* nb_teh_list = NULL;
static unsigned long nb_teh_count = 0;

static key_event_handler* nb_keh_list = NULL;
static unsigned long nb_keh_count = 0;

int nbRegisterTextEventHandler(text_event_handler teh) {
	text_event_handler* new_list
		= realloc(
			nb_teh_list,
			++nb_teh_count * sizeof(*nb_teh_list)
		);
	if (new_list) {
		nb_teh_list = new_list;
		nb_teh_list[nb_teh_count - 1] = teh;
		return 0;
	} else {
		nb_teh_count--;
		return 1;
	}
}

int nbRegisterKeyEventHandler(key_event_handler keh) {
	key_event_handler* tr
		= realloc(
			nb_keh_list,
			++nb_keh_count * sizeof(*nb_keh_list)
		);
	if (tr) {
		nb_keh_list = tr;
		nb_keh_list[nb_keh_count - 1] = keh;
		return 0;
	} else {
		nb_keh_list--;
		return 1;
	}
}

#if _WIN32
#define UNICODE
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN
#undef UNICODE

#define NB_WNDCLASS_NAME L"nb_window"

HWND main_window;
HGLRC main_glc;
HDC main_dc;

int errval;

int nbGetKeyState(enum nb_key k) {
	return (GetFocus() == main_window) ? (int)GetKeyState((int)k) & 0x8000 : 0;
}

LRESULT CALLBACK wndproc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
	if (message == WM_CLOSE)
		window_state = 0;
	if (message == WM_KEYDOWN) {
		for (unsigned long i = 0; i < nb_keh_count; i++) {
			nb_keh_list[i]((enum nb_key)wparam);
		}
	}
	if (message == WM_CHAR) {
		for (unsigned long i = 0; i < nb_teh_count; i++) {
			nb_teh_list[i]((char)wparam);
		}
	}
	return DefWindowProc(window, message, wparam, lparam);
}

void nbGetMousePosition(long* x, long* y) {
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(main_window, &p);
	p.y = (long)screen_height / 2 - p.y;
	p.x -= (long)screen_width / 2;
	if (x) *x = p.x;
	if (y) *y = p.y;
}

void nbPollEvent() {
	MSG roger = { 0 };
	if (PeekMessage(&roger, 0, 0, 0, PM_REMOVE)) {
		TranslateMessage(&roger);
		DispatchMessage(&roger);
	}
}

LONGLONG time_since_init;

milliseconds nbGetTicks() {
	milliseconds retval;
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	retval = (milliseconds)(li.QuadPart-time_since_init);
	retval /= 10000;
	
	return retval;
}

void nbQuit() {
	wglMakeCurrent(0, 0);
	wglDeleteContext(main_glc);
	if (nb_keh_count) {
		free(nb_keh_list);
		nb_keh_count = 0;
		nb_keh_list = NULL;
	}
	if (nb_teh_count) {
		free(nb_teh_list);
		nb_teh_count = 0;
		nb_teh_list = NULL;
	}
	UnregisterClass(NB_WNDCLASS_NAME, 0);
}

void nbSwapBuffers() {
	SwapBuffers(main_dc);
}

float screen_height, screen_width;

int nbInit(const wchar_t* title, unsigned long width, unsigned long height) {
	screen_height = (float)height;
	screen_width = (float)width;

	// Window class registration

	WNDCLASS wnd_class = { 0 };
	memset(&wnd_class, 0, sizeof(WNDCLASS));
	wnd_class.lpszClassName = NB_WNDCLASS_NAME;
	wnd_class.lpfnWndProc = wndproc;
	wnd_class.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wnd_class.hIcon = LoadCursor(GetModuleHandle(0), IDI_APPLICATION);
	wnd_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	wnd_class.style = CS_OWNDC;
	
	if (!RegisterClass(&wnd_class)) {
		fprintf(stderr, "NBML: Could not register window class.\r\n");
		return 0xFF01;
	}

	// Main window creation

	const DWORD win_style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

	RECT window_rect = { 0 };
	window_rect.left = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
	window_rect.top = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	window_rect.bottom = window_rect.top + height;
	window_rect.right = window_rect.left + width;

	AdjustWindowRect(&window_rect, win_style, 0);

	main_window = CreateWindow(
		NB_WNDCLASS_NAME,
		title,
		win_style,
		window_rect.left, window_rect.top,
		window_rect.right - window_rect.left,
		window_rect.bottom - window_rect.top,
		0, 0, GetModuleHandle(0), 0
	);
	
	if (main_window == 0) {
		fprintf(stderr, "NBML: Could not create main window.\r\n");
		return 0xFF02;
	}

	// OpenGL context creation

	main_dc = GetDC(main_window);
	if (main_dc == 0) {
		fprintf(stderr, "NBML: Could not create device context from main window.\r\n");
		return 0xFF03;
	}

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
		PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
		32,                   // Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                   // Number of bits for the depthbuffer
		8,                    // Number of bits for the stencilbuffer
		0,                    // Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	int pfid = ChoosePixelFormat(main_dc, &pfd);
	if (pfid == 0) {
		fprintf(stderr, "NBML: Could not find suitable pixel format for window.\r\n");
		return 0xFF04;
	}

	if (!SetPixelFormat(main_dc, pfid, &pfd)) {
		fprintf(stderr, "NBML: Could not set the pixel format.\r\n");
		return 0xFF05;
	}

	main_glc = wglCreateContext(main_dc);
	if (!main_glc) {
		fprintf(stderr, "NBML: Could not create gl context.\r\n");
		return 0xFF06;
	}
	wglMakeCurrent(main_dc, main_glc);

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "NBML: Could not initialize glew. (%s)\r\n", glewGetErrorString(err));
		return 0xFF07;
	}

	ShowWindow(main_window, SW_SHOW);
	window_state = 1;
	
	pass(graphicsInitGL());

	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	time_since_init = li.QuadPart;

	return 0;
}
#else
#error Other OS unimplemented
#endif
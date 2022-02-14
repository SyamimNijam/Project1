#include "nbml_internal.h"
#include "graphics.h"

#define btf(x) ((float)x / 255.0f)

unsigned int uni_translate;
unsigned int uni_scale;
unsigned int uni_tint;

static pixels nb_translate_x = 0;
static pixels nb_translate_y = 0;
static float nb_scale = 1.0f;

void nbSetTranslate(pixels x, pixels y) {
	nb_translate_x = x;
	nb_translate_y = y;
}
pixels nbGetTranslateX() { return nb_translate_x; }
pixels nbGetTranslateY() { return nb_translate_y; }

void nbClear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
void nbSetTint(byte r, byte g, byte b, byte a) {
	glUniform4f(uni_tint, btf(r), btf(g), btf(b), btf(a));
}
void nbClearColor(byte r, byte g, byte b) {
	glClearColor(btf(r), btf(g), btf(b), 1.0f);
}
void nbSetScale(float s) { nb_scale = s; }
float nbGetScale() { return nb_scale; }

void nbDrawRect(pixels x, pixels y, pixels w, pixels h, float depth) {
	glUniform2f(uni_scale,
		((float)w * 2) / screen_width * nb_scale,
		((float)h * 2) / screen_height * nb_scale
	);
	glUniform3f(uni_translate,
		((float)(nb_translate_x + x) * 2) / screen_width * nb_scale,
		((float)(nb_translate_y + y) * 2) / screen_height * nb_scale,
		depth
	);
	glBindVertexArray(box_vao);
	glDrawArrays(GL_QUADS, 0, 4);
#if _DEBUG
	GLenum e = glGetError();
	if (e != GL_NO_ERROR)
		fprintf(stderr, "GL Error %d in " __FUNCTION__ ".\r\n", e);
#endif
}

#include "nbml_internal.h"

int graphicsInitGL() {
	pass(graphicsInitShader());
	pass(graphicsInitMemory());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0f);

	return 0;
}
#pragma once
#define GLEW_STATIC
#include "GLEW/glew.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/GL.h>

extern int errval;
#define pass(x) errval = x; if (errval) return errval

extern float screen_height, screen_width;

extern unsigned int uni_texture_portion;
extern unsigned int uni_translate;
extern unsigned int uni_scale;
extern unsigned int uni_tint;

extern unsigned int fragment_shader;
extern unsigned int shader_program;
extern unsigned int vertex_shader;

extern unsigned int box_vao;

int graphicsInitMemory();
int graphicsInitShader();
int graphicsInitGL();
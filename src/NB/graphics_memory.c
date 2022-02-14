#include "nbml_internal.h"
#include "graphics.h"

#define LENGTH 0.5f
float box_position_data[] = {
	-LENGTH, +LENGTH,
	+LENGTH, +LENGTH,
	+LENGTH, -LENGTH,
	-LENGTH, -LENGTH,
};

float box_texcoord_data[] = {
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
};

unsigned int box_position_vbo;
unsigned int box_texcoord_vbo;
unsigned int box_vao;

void nbSetTextureId(image_id i) {
	glBindTexture(GL_TEXTURE_2D, i);
}

void nbSetTexture(image i) {
	nbSetTextureId(i.id);
}

void nbSetCropArea(float x, float y, float w, float h) {
	float l, r, t, b;
	l = x;
	r = x + w;
	t = y + h;
	b = y;
	float tex_data[] = {
		l, t,
		r, t,
		r, b,
		l, b
	};

	glBindBuffer(GL_ARRAY_BUFFER, box_texcoord_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tex_data), tex_data, GL_DYNAMIC_DRAW);
}

int graphicsInitMemory() {
	glGenVertexArrays(1, &box_vao);
	glBindVertexArray(box_vao);

	glGenBuffers(1, &box_position_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, box_position_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(box_position_data), box_position_data, GL_STATIC_DRAW);
	int attrib_position = glGetAttribLocation(shader_program, "position");
	glVertexAttribPointer(attrib_position, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attrib_position);

	glGenBuffers(1, &box_texcoord_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, box_texcoord_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(box_texcoord_data), box_texcoord_data, GL_DYNAMIC_DRAW);
	int attrib_texcoord = glGetAttribLocation(shader_program, "texcoord");
	glVertexAttribPointer(attrib_texcoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attrib_texcoord);

	uni_translate = glGetUniformLocation(shader_program, "move");
	glUniform3f(uni_translate, 0.0f, 0.0f, 0.0f);

	uni_scale = glGetUniformLocation(shader_program, "scale");
	glUniform2f(uni_scale, 1.0f, 1.0f);

	uni_tint = glGetUniformLocation(shader_program, "tint");
	glUniform4f(uni_tint, 1.0f, 1.0f, 1.0f, 1.0f);

	return 0;
}

image nbLoadImageFromBytes(image_data data, pixels height, pixels width) {
	image retval = { .id = 0, .height = height, .width = width };
	glGenTextures(1, &retval.id);
	glBindTexture(GL_TEXTURE_2D, retval.id);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width, (GLsizei)height,
		0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	GLenum errval = glGetError();
	if (errval != GL_NO_ERROR) {
		fprintf(stderr, "Could not load texture. GL Error code: %X.\r\n", errval);
		return (image){ 0 };
	}
	else {
		return retval;
	}
}
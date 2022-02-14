#include "nbml_internal.h"

char* vertex_shader_src =
	"#version 150 core\n"
	"in vec2 position;\n"
	"in vec2 texcoord;\n"
	"uniform vec2 scale;\n"
	"uniform vec3 move;\n"
	"uniform vec4 tint;"
	"out vec2 tex;\n"
	"out vec4 ttint;\n"
	"void main() {\n"
	"\ttex = texcoord;\n"
	"\tttint = tint;\n"
	"\tgl_Position = vec4(position, 0.0, 1.0)*vec4(scale, 1.0, 1.0)+vec4(move, 0.0);\n"
	"}"
;

char* fragment_shader_src =
	"#version 150 core\n"
	"in vec2 tex;\n"
	"in vec4 ttint;"
	"uniform sampler2D tex_data;\n"
	"out vec4 color;\n"
	"void main() {\n"
	"\tcolor = texture(tex_data, tex) * ttint;\n"
	"}"
;

int compileShader(unsigned int, char*);

unsigned int shader_program;

unsigned int fragment_shader;
unsigned int vertex_shader;

int graphicsInitShader() {
	// Shader compilation
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	pass(compileShader(vertex_shader, vertex_shader_src));
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	pass(compileShader(fragment_shader, fragment_shader_src));

	shader_program = glCreateProgram();
	glAttachShader(shader_program, fragment_shader);
	glAttachShader(shader_program, vertex_shader);
	glBindFragDataLocation(shader_program, 0, "color");
	glLinkProgram(shader_program);
	glUseProgram(shader_program);

	return 0;
}

int compileShader(unsigned int shader, char* src) {
	int compilation_status;
	char errmsg[1024];
	glShaderSource(shader, 1, (const GLchar* const*)&src, 0);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compilation_status);
	glGetShaderInfoLog(shader, sizeof(errmsg), 0, errmsg);
	if (compilation_status == GL_FALSE) {
		fprintf(stderr, "NBML: Failed to compile shader:\r\n%s\r\n\r\n", errmsg);
		return 0xFE01;
	}
	return 0;
}
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <assert.h>

#include "./util.h"
#include "./render.h"

static Shader current_shader = -1;
static unsigned int mesh = -1;

Texture EngLoadTexture(const char* path) {
	Texture r;
	int z;
	stbi_uc* raw = stbi_load(path, &r.w, &r.h, &z, 4);

	glGenTextures(1, &r.id);
	glBindTexture(GL_TEXTURE_2D, r.id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, r.w, r.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, raw);

	free(raw);

	return r;
}

Shader EngLoadShader(const char* vspath, const char* pspath) {
	const char* vssrc = UtilReadTextFile(vspath);

	Shader vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vssrc, NULL);
	glCompileShader(vs);

	int compiled;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &compiled);
	if (compiled != GL_TRUE) {
		int length;
		glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &length);

		char* info = malloc(length);
		glGetShaderInfoLog(vs, length, NULL, info);

		printf("'%s' failed to compile!\n%s", vspath, info);
	}

	const char* pssrc = UtilReadTextFile(pspath);

	Shader ps = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(ps, 1, &pssrc, NULL);
	glCompileShader(ps);

	glGetShaderiv(ps, GL_COMPILE_STATUS, &compiled);
	if (compiled != GL_TRUE) {
		int length;
		glGetShaderiv(ps, GL_INFO_LOG_LENGTH, &length);

		char* info = malloc(length);
		glGetShaderInfoLog(ps, length, NULL, info);

		printf("'%s' failed to compile!\n%s", pspath, info);
	}

	Shader program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, ps);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &compiled);
	if (compiled != GL_TRUE) {
		int length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

		char* info = malloc(length);
		glGetProgramInfoLog(program, length, NULL, info);

		printf("program failed to link!\n%s", info);
	}

	glDetachShader(program, vs);
	glDetachShader(program, ps);

	glDeleteShader(vs);
	glDeleteShader(ps);

	free((char*)vssrc);
	free((char*)pssrc);

	return program;
}

void EngSetShader(Shader shader) {
	current_shader = shader;
	glUseProgram(shader);
}

void EngDrawTexture(Texture tex, float x, float y) {
	assert(current_shader != (unsigned int)-1);

	float width = (float)(EngGetWidth());
	float height = (float)(EngGetHeight());
	float glx = x / width;
	float gly = y / height;
	float glw = (float)(tex.w) / width;
	float glh = (float)(tex.h) / height;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex.id);

	int location = glGetUniformLocation(current_shader, "tex");
	glUniform1i(location, GL_TEXTURE0);

	location = glGetUniformLocation(current_shader, "bounds");
	glUniform4f(location, glx, gly, glw, glh);

	if (mesh == (unsigned int)-1) {
		// generate quad mesh (+ texcoords)
		float verts[] = {
			-1.0, -1.0, 0.0, 0.0,
			-1.0,  1.0, 0.0, 1.0,
			 1.0,  1.0, 1.0, 1.0,

			 1.0,  1.0, 1.0, 1.0,
			-1.0, -1.0, 0.0, 0.0,
			 1.0, -1.0, 1.0, 0.0,

		};

		glGenBuffers(1, &mesh);
		glBindBuffer(GL_ARRAY_BUFFER, mesh);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), &verts, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, mesh); // TODO: is this needed?
		
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	}

	glBindBuffer(GL_ARRAY_BUFFER, mesh);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}


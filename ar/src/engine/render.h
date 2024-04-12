#pragma once

typedef struct {
	int w, h;
	unsigned int id;
} Texture;
typedef unsigned int Shader;

Texture EngLoadTexture(const char* path);
Shader EngLoadShader(const char* vspath, const char* pspath);

int EngGetWidth(void);
int EngGetHeight(void);

void EngSetShader(Shader shader);
void EngDrawTexture(Texture tex, float x, float y);


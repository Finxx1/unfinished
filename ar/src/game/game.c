#include "../engine/render.h"
#include "../engine/input.h"

#include <stdio.h>

Texture tex;
Shader shader;

void start() {
	printf("Game started!\n");
	printf("Loading textures...\n");

	tex = EngLoadTexture("res/cool1.png");
	shader = EngLoadShader("res/textured.vs", "res/textured.ps");

	EngSetShader(shader);
}

void update() {
	int mx, my;
	EngGetMouseXY(&mx, &my);
	EngDrawTexture(tex, mx, my);
	return;
}


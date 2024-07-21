#include "../engine/render.h"
#include "../engine/input.h"

#include <stdio.h>

#include "ecs.h"

Texture tex;
Shader shader;

Entity* ecs; // TODO: Allocate this and also do other stuff like depth testing

void mouse(int button, int action) {
	
}

void start() {
	printf("Game started!\n");
	printf("Allocating resources...\n");

	printf("Loading resources...\n");

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


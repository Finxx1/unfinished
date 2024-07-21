#pragma once

int EngGetMouseX(void);
int EngGetMouseY(void);
void EngGetMouseXY(int* x, int* y);

void EngSetMouseCallback(void (*func)(int, int));

enum {
	ENG_MOUSE_LEFT,
	ENG_MOUSE_RIGHT,
	ENG_MOUSE_MIDDLE,
};

enum {
	ENG_BUTTON_UP,
	ENG_BUTTON_DOWN,
	ENG_BUTTON_REPEAT,
};


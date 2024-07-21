#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>

#include <stb_image.h>

#include "../config.h"
#include "./render.h"
#include "./input.h"

extern void start(void);
extern void update(void);

static GLFWwindow* window;
static void (*mousecallback)(int, int) = NULL;

int EngGetWidth(void) {
	int width;
	glfwGetWindowSize(window, &width, NULL);
	return width;
}

int EngGetHeight(void) {
	int height;
	glfwGetWindowSize(window, NULL, &height);
	return height;
}

int EngGetMouseX(void) {
	double mx;
	glfwGetCursorPos(window, &mx, NULL);
	return floor(mx);
}

int EngGetMouseY(void) {
	double my;
	glfwGetCursorPos(window, &my, NULL);
	return EngGetHeight() - floor(my);
}

void EngGetMouseXY(int* x, int* y) {
	double mx, my;
	glfwGetCursorPos(window, &mx, &my);
	*y = EngGetHeight() - floor(my);
	*x = floor(mx);
}

void EngSetMouseCallback(void (*func)(int, int)) {
	mousecallback = func;
}

void glfwmousecallback(GLFWwindow* window, int button, int action, int mods) {
	if (mousecallback) mousecallback(button, action);
	(void)(window);
	(void)(mods);
}

void onexit(void) {
	glfwTerminate();
}

int main() {
	assert(glfwInit());

	atexit(onexit);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(ENGINE_WIDTH, ENGINE_HEIGHT, ENGINE_TITLE, NULL, NULL);
	assert(window);

	glfwMakeContextCurrent(window);

	assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));

	glEnable(GL_TEXTURE_2D);

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	stbi_set_flip_vertically_on_load(1);

	glfwSetMouseButtonCallback(window, glfwmousecallback);

	start();

	while (!glfwWindowShouldClose(window)) {
		double starttime = glfwGetTime();
		
		glClear(GL_COLOR_BUFFER_BIT);

		update();

		glfwSwapBuffers(window);
		glfwPollEvents();

		while (glfwGetTime() < starttime + ENGINE_FRAMEDELTA) ;
	}

	return 0;
}


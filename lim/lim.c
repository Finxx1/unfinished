#include <stdio.h>
#include <stdlib.h>

#include "x.h"

#define die(...) { fprintf(stderr, __VA_ARGS__); exit(1); } do {} while(0)

void resize(int w, int h) {

}

void kb(unsigned long code) {
	printf("0x%x\n", code);
}

int main() {
	create_window(800, 600);
	
	set_resize_callback(resize);
	set_kb_callback(kb);

	loop_window();

	return 0;
}

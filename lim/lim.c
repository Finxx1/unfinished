#include <stdio.h>
#include <stdlib.h>

#include "x.h"

#define die(...) { fprintf(stderr, __VA_ARGS__); exit(1); } do {} while(0)

int w, h; // width and height in cells
int cx = 0, cy = 0; // cursor position in cells

typedef struct {
	char* raw;
	int size;
} editor_row;

typedef struct {
	editor_row* rows;
	int rowcount;
} editor_file;

void draw() {
	clear_window();
	draw_line("                        f->badweight = 1;", 1, 0, 0);
	draw_cursor(cx, cy);
}

void resize(int w, int h) {
	draw();
}

void kb(int code) {
	switch (code) {
		case XWrapKLEFT:
			cx--;
			break;
		case XWrapKRIGHT:
			cx++;
			break;
		case XWrapKDOWN:
			cy++;
			break;
		case XWrapKUP:
			cy--;
			break;
	}
	draw();
}

int main() {
	set_bg(0xB3000000); // Note: ARGB, premultiplied!
	create_window(800, 600);
	
	set_resize_callback(resize);
	set_kb_callback(kb);
	set_draw(draw);

	// Feel free to configure these yourself
	set_border(5);
	set_font("Monospace:pixelsize=16:antialias=true:autohint=true");
	set_colors("#FFFFFF", "#63f6ad", "#5ed3fa");

	loop_window();

	return 0;
}

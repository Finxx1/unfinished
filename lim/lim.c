#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "x.h"
#include "util.h"

#define die(...) { fprintf(stderr, __VA_ARGS__); exit(1); } do {} while(0)

int w, h; // width and height in cells
int cx = 0, cy = 0; // cursor position in cells
int scroll = 0, hscroll = 0;

typedef struct {
	char* raw;
	int size;
	size_t capacity;
} editor_row;

typedef struct {
	editor_row* rows;
	int rowcount;
} editor_file;

editor_file file;

void draw() {
	clear_window();

	for(int i = 0; i < file.rowcount; i++) {
		// check for comments
		int commentindex = -1;
		for (int j = 0; j < file.rows[i].size - 1; j++) {
			if (file.rows[i].raw[j] == '/' && file.rows[i].raw[j + 1] == '/') {
				commentindex = j;
				break;
			}
		}

		if (commentindex == -1) {
			draw_line(file.rows[i].raw, file.rows[i].size, 0, 0, i + scroll);
		} else {
			draw_line(file.rows[i].raw, commentindex, 0, hscroll, i + scroll);
			draw_line(file.rows[i].raw + commentindex, file.rows[i].size - commentindex, 2, hscroll + commentindex, i + scroll);
		}
	}

	draw_cursor(cx + hscroll, cy + scroll);
}

void resize(int w, int h) {
	draw();
}

void kb(int code) {
	printf("0x%x\n", code);

	switch (code) {
		case XWrapKLEFT:
			if (cx == 0) break;
			cx--;
			break;
		case XWrapKRIGHT:
			if (cx == file.rows[cy].size) break;
			cx++;
			break;
		case XWrapKDOWN:
			if (cy + 1 == file.rowcount) break;
			cy++;
			break;
		case XWrapKUP:
			if (cy == 0) break;
			cy--;
			break;
		case XWrapMWheelUp:
			scroll--;
			break;
		case XWrapMWheelDown:
			scroll++;
			break;
		case XWrapKBackspace:
			if (cx == 0) break;
			
			cx--;

			del_char(file.rows[cy].raw, file.rows[cy].capacity, cx);
			file.rows[cy].size--;
			
			break;
		default:
			if (code > 255) break;
			file.rows[cy].size++;
			file.rows[cy].capacity++;
			file.rows[cy].raw = realloc(file.rows[cy].raw, file.rows[cy].capacity);

			insert_char(file.rows[cy].raw, file.rows[cy].capacity, cx, code);
			cx++;
			break;
	}

	draw();
}

void new_file() {
	file.rowcount = 1;
	file.rows = calloc(1, sizeof(editor_row));
}

void read_file(char* file_name) {
	FILE* fp = fopen(file_name, "rb");
	if (!fp) die("can't open file\n");

	// first pass to see how many lines there are
	int c;
	int lines = 0;
	while ((c = fgetc(fp)) != EOF)
		if (c == '\n') lines++;

	file.rowcount = lines;
	file.rows = calloc(lines, sizeof(editor_row));
	if (!file.rows) die("can't alloc memory");

	// actually read bytes this time
	fseek(fp, 0, SEEK_SET);

	for (int i = 0; i < lines; i++) {
		file.rows[i].size = getline(&file.rows[i].raw, &file.rows[i].capacity, fp);
	}

	fclose(fp);

	// go through and replace tabs with spaces
	for (int i = 0; i < lines; i++) {
		file.rows[i].raw = str_replace(file.rows[i].raw, "\t", "    ");
		file.rows[i].size = strlen(file.rows[i].raw);
		file.rows[i].capacity = strlen(file.rows[i].raw);
	}
}

int main(int argc, char* argv[]) {
	if (argc > 1) read_file(argv[1]);
	else new_file();

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

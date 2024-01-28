// You thought that WinAPI was bad? Oh, you sweet, sweet summer child.
// X is by far the worst API I have had the displeasure of working with.
// I have made several WinAPI projects, endured Apple's unwillingness to
// use anything but Objective-C, but this... this almost broke me.
// This library is by far the oldest thing I have ever seen. Xorg traces
// its roots to Xfree86, a project as old as Linux itself. X11 (the API)
// was released back in 1987. 1987. That is nearly four decades old. This
// API was made back when the USSR was still around. I genuinely offer my
// sincerest condolences to all the thousands of unfortunate developers
// who wasted their manpower on this awful project. I can't wait until
// Wayland finally takes over. Until then, this file provides a wrapper
// of sorts around X.


#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>

#include <stdio.h>
#include <stdlib.h>

#include "x.h"

#define die(...) { fprintf(stderr, __VA_ARGS__); exit(1); } do {} while(0)

static Display* d;
static int root;
static int scr;
static Colormap cmap;
static Visual* vis;
static Window w;
static GC gc;

static int thickness;

// Set distance between border and text
void set_border(int _thickness) {
	thickness = _thickness;
}

// Callback when Expose is posted
static void (*draw)(void) = NULL;
void set_draw(void (*callback)(void)) {
	draw = callback;
}

// Callback when KeyPress is posted
static void (*kb)(int) = NULL;
void set_kb_callback(void (*callback)(int)) {
	kb = callback;
}

// Callback when window is resized
static void (*resize)(int, int) = NULL;
void set_resize_callback(void (*callback)(int, int)) {
	resize = callback;
}

// Sets the background color
static unsigned long bgcolor = 0xFF000000;
void set_bg(unsigned long color) {
	bgcolor = color;
}

static XftDraw* xftdraw;
static XftFont* font;
static XftColor regular;
static XftColor comment;
static XftColor bold;

// Sets the font
void set_font(const char* name) {
	font = XftFontOpenName(d, scr, name);
	if (!font) die("can't load font\n");
}

// Sets the colors
void set_colors(const char* def, const char* _comment, const char* _bold) {
	if (!XftColorAllocName(d, vis, cmap, def, &regular))
		die("cannot allocate regular color\n");
	if (!XftColorAllocName(d, vis, cmap, _comment, &comment))
		die("cannot allocate comment color\n");
	if (!XftColorAllocName(d, vis, cmap, _bold, &bold))
		die("cannot allocate bold color\n");
}

// Draw a line of text in the given color
void draw_line(char* txt, int color, int x, int y) {
	XftColor* c = &regular;
	switch (color) {
		case 1:
			c = &comment;
			break;
		case 2:
			c = &bold;
			break;
	}

	XftDrawStringUtf8(xftdraw, c, font, thickness + x * get_font_width(), thickness + (y + 1) * font->ascent, txt, strlen(txt));
}

// Draw a cursor at the position
void draw_cursor(int x, int y) {
	XSetForeground(d, gc, regular.pixel);
	XFillRectangle(d, w, gc, thickness + x * get_font_width(), thickness + y * get_font_height(), 1, get_font_height());
}

// Gets the width/heigth of a character
int get_font_width() {
	XGlyphInfo i;
	XftTextExtents8(d, font, "M", 1, &i);
	return i.width;
}
int get_font_height() {
	return font->ascent + font->descent;
}

// Clears the window
void clear_window() {
	XClearWindow(d, w);
}

// Create window
void create_window(int width, int height) {
	d = XOpenDisplay(NULL);
	if (!d) die("can't open display\n");

	root = XDefaultRootWindow(d);
	scr = XDefaultScreen(d);

	XVisualInfo visinfo = {};
	if (!XMatchVisualInfo(d, scr, 32, TrueColor, &visinfo)) die ("upgrade your monitor\n");

	cmap = XCreateColormap(d, root, visinfo.visual, AllocNone);
	vis = visinfo.visual;

	XSetWindowAttributes winattr;
	winattr.bit_gravity = StaticGravity;
	winattr.background_pixel = bgcolor; 
	winattr.colormap = cmap;
	winattr.event_mask = StructureNotifyMask | KeyPressMask;
	unsigned long winattrmask = CWBitGravity | CWBackPixel | CWColormap | CWBorderPixel | CWEventMask;

	w = XCreateWindow(d, root, 0, 0, width, height, 0, visinfo.depth, InputOutput, visinfo.visual, winattrmask, &winattr);
	if (!w) die("can't create window\n");
	gc = XCreateGC(d, w, 0, NULL);

	XStoreName(d, w, "lim");

	xftdraw = XftDrawCreate(d, w, vis, cmap);

	XMapWindow(d, w);

	XFlush(d);
}

void loop_window(void) {
	draw(); // For some reason, Expose isn't sent...

	int open = 1;
	XEvent e = {};
	while (open) {
		XNextEvent(d, &e);
		if (e.type == DestroyNotify) {
			open = 0;
		}
		if (e.type == ResizeRequest) {
			if (resize)
				resize(e.xresizerequest.width, e.xresizerequest.height);
		}
		if (e.type == Expose) {
			if (draw)
				draw();
		}
		if (e.type == KeyPress) {
			if (kb) {
				char tmp[32]; // 32 seems like a good bet
				KeySym k;
				XLookupString(&e.xkey, tmp, 32, &k, NULL);
				
				if (tmp[0] == 0)
					kb(k);
				else
					kb(tmp[0]);
			}
		}
	}
}


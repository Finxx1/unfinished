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

#define die(...) { fprintf(stderr, __VA_ARGS__); exit(1); } do {} while(0)

static Display* d;
static int root;
static int scr;
static Window w;

// Callback when Expose is posted
static void (*draw)(void) = NULL;
void set_draw(void (*callback)(void)) {
	draw = callback;
}

// Callback when KeyPress is posted
static void (*kb)(unsigned long) = NULL;
void set_kb_callback(void (*callback)(unsigned long)) {
	kb = callback;
}

// Callback when window is resized
static void (*resize)(int, int) = NULL;
void set_resize_callback(void (*callback)(int, int)) {
	resize = callback;
}

// Create window
void create_window(int w, int h) {
	d = XOpenDisplay(NULL);
	if (!d) die("can't open display\n");

	root = DefaultRootWindow(d);
	scr = DefaultScreen(d);

	XVisualInfo visinfo = {};
	if (!XMatchVisualInfo(d, scr, 24, TrueColor, &visinfo)) die ("upgrade your monitor\n");

	XSetWindowAttributes winattr;
	winattr.bit_gravity = StaticGravity;
	winattr.background_pixel = 0;
	winattr.colormap = XCreateColormap(d, root, visinfo.visual, AllocNone);
	winattr.event_mask = StructureNotifyMask | KeyPressMask | ResizeRedirectMask;
	unsigned long winattrmask = CWBitGravity | CWBackPixel | CWColormap | CWEventMask;

	w = XCreateWindow(d, root, 0, 0, w, h, 0, visinfo.depth, InputOutput, visinfo.visual, winattrmask, &winattr);
	if (!w) die("can't create window\n");

	XStoreName(d, w, "lim");

	XMapWindow(d, w);
	XFlush(d);
}

void loop_window(void) {
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
				
			}
		}
	}
}

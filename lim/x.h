// Wrapper around Xlib (not intended for other applications!)

// Keycodes
#define XWrapKLEFT 0xFF51
#define XWrapKRIGHT 0xFF53
#define XWrapKDOWN 0xFF54
#define XWrapKUP 0xFF52

// Set distance between border and text
void set_border(int thickness);

// Callback when Expose is posted
void set_draw(void (*callback)(void));

// Callback when KeyPress is posted
void set_kb_callback(void (*callback)(int));

// Callback when window is resized
void set_resize_callback(void (*callback)(int, int));

// Sets the background color
void set_bg(unsigned long color);

// Sets the font
void set_font(const char* name);

// Sets the colors
void set_colors(const char* def, const char* comment, const char* bold);

// Draw a line of text in the given color
void draw_line(char* txt, int len, int color, int x, int y);

// Draw a cursor at the position
void draw_cursor(int x, int y);

// Gets the width/heigth of a character
int get_font_width();
int get_font_height();

// Clears the window
void clear_window();

// Create window
void create_window(int w, int h);

// Run window loop
void loop_window(void);


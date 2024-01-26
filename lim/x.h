// Wrapper around Xlib (not intended for other applications!)

// Callback when Expose is posted
void set_draw(void (*callback)(void));

// Callback when KeyPress is posted
void set_kb_callback(void (*callback)(int));

// Callback when window is resized
void set_resize_callback(void (*callback)(int, int));

// Create window
void create_window(int w, int h);

// Run window loop
void loop_window(void);

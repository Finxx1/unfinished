typedef struct Entity {
	void (*mouse)(struct Entity*, int, int);
	void (*keyboard)(struct Entity*, int, int);
	void (*start)(struct Entity*);
	void (*update)(struct Entity*);
	void (*draw)(struct Entity*);
	int x;
	int y;
	int width;
	int height;
} Entity;

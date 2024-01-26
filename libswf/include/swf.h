// libswf is licensed under BSD-2 Clause (See LICENSE for more info)

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef struct {
	int x1;
	int y1;
	int x2;
	int y2;
} swf_rect;

swf_rect swf_read_rect(uint8_t* bytes);


typedef struct {
	uint16_t type;
	uint32_t length;
	void* tag;
} swf_tag;

typedef struct {
	size_t tagcount;
	swf_tag* tags;
} swf_tag_list;


typedef struct {
	uint8_t version;
	swf_rect frame;
	int framerate;
	int framecount;
} swf;

swf swf_read(const char* filename);
swf swf_read_memory(uint8_t* bytes, size_t length);

char* swf_error();

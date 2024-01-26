// libswf is licensed under BSD-2 Clause (See LICENSE for more info)

#include <swf.h>

#include "util.h"

#include <stdio.h>
#include <stdlib.h>

char* swf_errors[] = {
	"",
	"failed to open file",
	"failed to alloc memory",
	"failed to read file data",
};
size_t swf_errno = 0;

swf swf_read(const char* filename) {
	swf r = {0};

	FILE* fp = fopen(filename, "rb");

	if (!fp) {
		swf_errno = 1;
		return r;
	}

	fseek(fp, 0, SEEK_END);
	long len = ftell(fp);
	rewind(fp);

	uint8_t* bytes = malloc(len);

	if (!bytes) {
		swf_errno = 2;
		fclose(fp);
		return r;
	}

	if (fread(bytes, 1, len, fp) != len) {
		swf_errno = 3;
		fclose(fp);
		return r;
	}

	r = swf_read_memory(bytes, len);

	fclose(fp);

	return r;
}

swf swf_read_memory(uint8_t* bytes, size_t length) {
	swf r;

	char compression = bytes[0];

	uint8_t* raw = bytes;
	size_t len = length;

	if (compression == 'C') {
		swf__zlib_decompress(bytes, len, &raw, &length);
	}

	r.version = bytes[4];
}

char* swf_error() {
	return swf_errors[swf_errno];
}

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

static inline char* UtilReadTextFile(const char* name) {
	FILE* fp = fopen(name, "r");
        assert(fp);
        fseek(fp, 0, SEEK_END);
        size_t len = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        char* r = malloc(len + 1);
        assert(r);
        assert(fread(r, 1, len, fp) == len);
        r[len] = '\0';

	return r;
}


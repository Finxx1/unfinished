// libswf is licensed under BSD-2 Clause (See LICENSE for more info)

#include "util.h"

#include <zlib.h>

bool swf__zlib_decompress(uint8_t* bytes, size_t length, uint8_t** out_bytes, size_t* out_length) {
	z_stream infstream = {0};
	infstream.avail_in = length - 8;
	infstream.next_in = bytes + 8;

	uint8_t* o_bytes = malloc(length);

	infstream.avail_out = length;
	infstream.next_out = o_bytes;

	inflateInit(&infstream);
	inflate(&infstream, Z_NO_FLUSH);
	inflateEnd(&infstream);

	o_bytes = realloc(o_bytes, infstream.total_out);

	*out_length = infstream.total_out;
	*out_bytes = o_bytes;

	return o_bytes != 0;
}

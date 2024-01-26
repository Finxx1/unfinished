// libswf is licensed under BSD-2 Clause (See LICENSE for more info)

#include <swf.h>

#include "util.h"

swf_rect swf_read_rect(uint8_t* bytes) {
	int nbits = swf__read_bits(bytes, 0, 5);

	swf_rect r;
	r.x1 = swf__read_bits_signed(bytes, 5, nbits);
	r.x2 = swf__read_bits_signed(bytes, 5 + nbits, nbits);
	r.y1 = swf__read_bits_signed(bytes, 5 + nbits * 2, nbits);
	r.y2 = swf__read_bits_signed(bytes, 5 + nbits * 3, nbits);

	return r;
}

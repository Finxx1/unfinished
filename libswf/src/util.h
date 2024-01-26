#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static inline uint64_t swf__read_bits(uint8_t* data, size_t bitoff, size_t bitlen) {
	uint64_t tmp = 0;
    if(bitlen == 0)
        return 0;
    unsigned bytes = bitoff >> 3,
             bits = bitoff & 7,
             read_bytes = (bitlen >> 3) + 2,
             inv_read = 8 - read_bytes;
    for(unsigned i = 7; i >= inv_read; --i){
        if(i == inv_read && (bitlen & 7) <= 8 - bits)
            break;
        tmp |= (int64_t)data[bytes++] << (i << 3);
    }

    return (tmp << bits) >> (64 - bitlen);
}

static inline int64_t swf__read_bits_signed(uint8_t* data, size_t bitoff, size_t bitlen) {
	uint64_t tmp = swf__read_bits(data, bitoff, bitlen);
	if(tmp >> (bitlen - 1))
        tmp |= (0xFFFFFFFFFFFFFFFF >> bitlen) << bitlen;
    return (int64_t)tmp;
}

bool swf__zlib_decompress(uint8_t* bytes, size_t length, uint8_t** out_bytes, size_t* out_length);
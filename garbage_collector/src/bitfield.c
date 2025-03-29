#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include "bitfield.h"


size_t bit_field_size(size_t num_bits){
	return (uint64_t)(ceilf((double)(num_bits)/(8.0*sizeof(*(((BitField*)0)->bits)))));
}


BitField bit_field_new(size_t num_bits){
	return (BitField){ calloc(sizeof((((BitField*)0)->bits)), bit_field_size(num_bits)) };
}


void bit_field_free(BitField b){
	free(b.bits);
}


bool bit_field_get(BitField b, size_t index){
	size_t sz = sizeof(*(b.bits))*8;
	return b.bits[index/sz] >> (index%sz) & 1;
}


void bit_field_set(BitField* b, size_t index, bool val){
	size_t sz = sizeof(*(b->bits))*8;
	BIT_FILED_TYPE* curr = &(b->bits[index/sz]);
	uint8_t offset = index%sz;
	*curr = (*curr & ~(1ULL<<offset)) | (((BIT_FILED_TYPE)val)<<offset);
}


void bit_field_clear(BitField* b, size_t size){
	memset(b->bits, 0, sizeof(*(b->bits)) * bit_field_size(size));
}
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "bitfield.h"


size_t bit_field_size(size_t num_bits){
	return (uint64_t)(ceilf((double)(num_bits)/(8.0*sizeof(*(((BitField*)0)->bits)))));
}


void bit_field_free(BitField b){
	free(b.bits);
}


BitField bit_field_new(size_t num_bits){
	return (BitField){ calloc(sizeof((((BitField*)0)->bits)), bit_field_size(num_bits)) };
}


bool bit_field_get(BitField b, MemLoc index){
	size_t sz = sizeof(*(b.bits))*8;
	return b.bits[index.x/sz] >> (index.x%sz) & 1;
}


void bit_field_set(BitField* b, MemLoc index, bool val){
	size_t sz = sizeof(*(b->bits))*8;
	uint64_t* curr = &(b->bits[index.x/sz]);
	uint8_t offset = index.x%sz;
	*curr = (*curr & ~(1ULL<<offset)) | (((uint64_t)val)<<offset);
}
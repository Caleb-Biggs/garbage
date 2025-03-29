#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include "bitfield.h"


size_t bit_field_size(size_t num_bits){
	return (uint64_t)(ceilf((double)(num_bits)/(8.0*sizeof(*(((BitField*)0)->bits)))));
}


// Returns NULL if malloc or calloc errors
BitField* bit_field_new(size_t num_bits){
	BitField* output = malloc(sizeof(*output));
	if(output == NULL) return NULL;
	output->bits =  calloc(sizeof((((BitField*)0)->bits)), bit_field_size(num_bits));
	if(output->bits == NULL) return NULL;
	return output;
}


void bit_field_free(BitField* b){
	if(b != NULL) free(b->bits);
	free(b);
}


// Assumes index is valid
// Returns 0 or 1 with a valid input, -1 otherwise
int bit_field_get(BitField b, size_t index){
	if(b.bits == NULL) return -1;
	size_t sz = sizeof(*(b.bits))*8;
	return b.bits[index/sz] >> (index%sz) & 1;
}


// Assumes index is valid
// Returns 0 with a valid input, -1 otherwise
int bit_field_set(BitField* b, size_t index, bool val){
	if(b == NULL || b->bits == NULL) return -1;
	size_t sz = sizeof(*(b->bits))*8;
	BIT_FILED_TYPE* curr = &(b->bits[index/sz]);
	uint8_t offset = index%sz;
	*curr = (*curr & ~(1ULL<<offset)) | (((BIT_FILED_TYPE)val)<<offset);
	return 0;
}


// Assumes size is correct
// Returns 0 with a valid input, -1 otherwise
int bit_field_clear(BitField* b, size_t size){
	if(b == NULL || b->bits == NULL) return -1;
	memset(b->bits, 0, sizeof(*(b->bits)) * bit_field_size(size));
	return 0;
}
#ifndef BIT_FIELD_H
#define BIT_FIELD_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "shared.h"

typedef struct BIT_FIELD {
	uint64_t* bits;
} BitField;


BitField bit_field_new(size_t size);
void bit_field_free(BitField b);
bool bit_field_get(BitField b, MemLoc index);
void bit_field_set(BitField* b, MemLoc index, bool val);

#endif
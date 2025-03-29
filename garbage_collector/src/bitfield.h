#ifndef BIT_FIELD_H
#define BIT_FIELD_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "shared.h"

#define BIT_FILED_TYPE uint8_t
typedef struct BIT_FIELD {
	BIT_FILED_TYPE* bits;
} BitField;


BitField bit_field_new(size_t size);
void bit_field_free(BitField b);
bool bit_field_get(BitField b, size_t index);
void bit_field_set(BitField* b, size_t index, bool val);
void bit_field_clear(BitField* b, size_t size);

#endif
#ifndef ARRAY_H
#define ARRAY_H

#include "types.h"

typedef struct ARRAY {
	bool is_pointer;
	TypeIndex type;
	size_t len;
	void** data;
} Array;

TypeIndex INIT_POINTER_ARR();
TypeIndex TYPE_POINTER_ARR();

Array array_new(TypeIndex t, size_t len);
Array array_new_pnt(size_t len);
void* array_new_fixed(TypeIndex t, size_t len);
void* array_get(Array a, size_t index);
void array_resize(Array* a, size_t len);

#endif
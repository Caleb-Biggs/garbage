#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "types.h"
#include "garbage.h"

/**
 * Type Functions
 **/

static size_t next_index = 0;
static size_t curr_size = TYPE_INIT_SIZE;
static TypeInfo* types = NULL;


TypeIndex type_init(size_t struct_sz, size_t num_memb, void** members){
	if(next_index >= curr_size || types == NULL){
		curr_size *= TYPE_INIT_RATIO;
		types = realloc(types, sizeof(*types)*curr_size);
	}

	size_t bytes = sizeof(*members) * num_memb;
	types[next_index] = (TypeInfo){struct_sz, num_memb, malloc(bytes)};
	memcpy(types[next_index].members, members, bytes);

	return (TypeIndex){next_index++};
}


TypeInfo* type_get_info(TypeIndex t){
	if(t.index >= next_index || !types) return NULL;
	return types+t.index;
}


void type_free(){
	for(size_t i = 0; i < next_index; i++)
		free(types[i].members);
	free(types);
}

/**
 * Array Functions
 **/

struct_setup(ARRAY, Array,
	type_memb(Array, data)
)


Array* array_new(TypeIndex t, size_t len){
	Array* output = gc_alloc(TYPE_ARRAY());
	void* arr = gc_alloc_array(t, len);
	if(!output || !arr) return NULL;

	output->type = t;
	output->len = len;
	output->data = arr;
	return output;
}


void* array_get(Array a, size_t index){
	return ((uint8_t*)a.data) + (index * type_get_info(a.type)->struct_sz);
}


void array_for_each(Array a, ssize_t num, void (*func)(void*)){
	size_t size = type_get_info(a.type)->struct_sz;
	if(num < 0 || num > a.len) num = a.len;
	num *= size;
	for(size_t i = 0; i < num; i+=size){
		func(((uint8_t*)a.data)+i);
	}
}


void array_resize(Array* a, size_t len){
	// printf("RESIZE\n");
	gc_realloc_array(a, len);
}


primitive_setup(POINTER, void*)
primitive_setup(BOOL, bool)
primitive_setup(CHAR, char)
primitive_setup(INT, int)
primitive_setup(FLOAT, float)
primitive_setup(DOUBLE, double)
primitive_setup(LONG_DOUBLE, long double)
primitive_setup(INT8, int8_t)
primitive_setup(INT16, int16_t)
primitive_setup(INT32, int32_t)
primitive_setup(INT64, int64_t)
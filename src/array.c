#include <string.h>
#include <stdio.h>
#include "array.h"
#include "garbage.h"


// primitive_setup(POINTER_ARR, void*)
struct_setup(ARRAY, Array,
	type_memb(Array, data)
)


Array* array_new(TypeIndex t, size_t len){
	// Array* output = malloc(type_get_info(TYPE_ARRAY())->struct_sz);
	Array* output = gc_alloc(TYPE_ARRAY());
	// void* arr = calloc(len, type_get_info(t)->struct_sz);
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


// TODO: probably broken
void array_resize(Array* a, size_t len){
	printf("RESIZE\n");
	size_t old_len = a->len;
	a->len = len;

	// void* new_data = calloc(len, type_get_info(a->type)->struct_sz);
	void* new_data = gc_alloc_array(a->type, len);
	
	size_t bytes = type_get_info(a->type)->struct_sz 
		* ((len<old_len) ? len: old_len);
	a->data = memcpy(new_data, a->data, bytes);
}


// void* array_new_fixed(TypeIndex t, size_t len){
// 	// return gc_alloc_array(t, len, (t.index == TYPE_POINTER().index));
// 	return NULL;
// }
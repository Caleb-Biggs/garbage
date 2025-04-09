#include <string.h>
#include <stdio.h>
#include "array.h"
#include "garbage.h"


primitive_setup(POINTER_ARR, void*)


Array array_new(TypeIndex t, size_t len){
	void** arr = gc_alloc_array(t, len, false);
	return (Array){false, t, len, arr};
}


Array array_new_pnt(size_t len){
	void** arr = gc_alloc_array(TYPE_POINTER(), len, true);
	return (Array){true, TYPE_POINTER_ARR(), len, arr};
	// void** arr = gc_alloc_array(TYPE_POINTER(), len);
	// return (Array){true, TYPE_POINTER(), len, arr};
}


void* array_get(Array a, size_t index){
	return *(uint8_t**)a.data + (index*type_get_info(a.type)->struct_sz);
}


// TODO: probably broken
void array_resize(Array* a, size_t len){
	printf("RESIZE\n");
	size_t old_len = a->len;
	a->len = len;
	void* new_data = gc_alloc_array(a->type, len, (a->type.index == TYPE_POINTER_ARR().index));
	size_t bytes = type_get_info(a->type)->struct_sz 
		* ((len<old_len) ? len: old_len);
	a->data = memcpy(new_data, a->data, bytes);
}


void* array_new_fixed(TypeIndex t, size_t len){
	return gc_alloc_array(t, len, (t.index == TYPE_POINTER().index));
}
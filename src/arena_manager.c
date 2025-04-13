#include <stdio.h>
#include <stdlib.h>
#include "arena_manager.h"


int manager_new(ArenaManager* output){
	if(!output) return -3;
	
	MetaArena* arena_size = calloc(STARTING_META_ARENAS+1, sizeof(*arena_size));
	if(!arena_size) return -4;
	for(size_t i = 0; i <= STARTING_META_ARENAS; i++){
		int ret = meta_arena_new(i, arena_size+i);
		if(ret < 0) return ret;
	}
	
	*output = (ArenaManager){
		.largest = STARTING_META_ARENAS,
		.arena_size = arena_size
	};
	return 0;
}


void manager_free(ArenaManager* a){
	for(size_t i = 0; i <= a->largest; i++){
		meta_arena_free(a->arena_size+i);
	}
	free(a->arena_size);
}


int manager_resize(ArenaManager* a, size_t size){
	a->arena_size = realloc(a->arena_size, (size+1)*sizeof(*(a->arena_size)));
	for(size_t i = a->largest+1; i <= size; i++){
		int ret = meta_arena_new(i, a->arena_size+i);
		if(ret < 0) return ret;
	}
	a->largest = size;
	return 0;
}


void* manager_allocate(ArenaManager* a, TypeIndex t){
	size_t size = type_get_info(t)->struct_sz;
	if(size > a->largest) {
		int ret = manager_resize(a, size);
		if(ret < 0) return NULL;
	}
	void* output = meta_arena_allocate(a->arena_size+size, t);
	// printf("manager allocating: %p\n", output);
	return output;
}


void* manager_allocate_arbitrary(ArenaManager* a, TypeIndex t, size_t size){
	if(size > a->largest) {
		int ret = manager_resize(a, size);
		if(ret < 0) return NULL;
	}
	void* output = meta_arena_allocate(a->arena_size+size, t);
	// printf("manager allocating: %p\n", output);
	return output;
}


void manager_delete_unmarked(ArenaManager a){
	for(size_t i = 0; i <= a.largest; i++)
		meta_arena_delete_unmarked(a.arena_size+i);
}


void manager_print(ArenaManager a){
	printf("Largest Arena Size: %lu\n", a.largest);
	for(size_t i = 0; i <= a.largest; i++){
		if(a.arena_size[i].num_arenas == 0) continue;
		printf("Size %lu\n", i);
		meta_arena_print(a.arena_size[i]);
		printf("\n");
	}
}
#include <stdlib.h>
#include "garbage.h"
#include "arena.h"


MemArr* memory = NULL;


MemLoc gc_alloc(size_t size){
	return mem_arr_insert_data(memory, malloc(size));
}


void attach(MemLoc parent, MemLoc child){
	HashSet* h = mem_arr_get_node(memory, parent)->refs;
	hash_set_insert(&h, child);
}


void detach(MemLoc parent, MemLoc child){

}


void* get(MemLoc index){
	return NULL;
}


void start_garbage_collection(){
	memory = mem_arr_new();
}


void end_garbage_collection(){
	mem_arr_free(&memory);
}


void start_function(){

}


void end_function(){

}
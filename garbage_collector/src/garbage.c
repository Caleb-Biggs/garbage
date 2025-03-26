#include <stdlib.h>
#include "garbage.h"
#include "arena.h"


/**
 * 
 * 
 */


typedef struct MEMORY_LIST {
	MemLoc index;
	struct MEMORY_LIST* next;
} List;


// Assumes l is not NULL, *l can be NULL
void list_push(List** l, MemLoc index){
	List* new = malloc(sizeof(*new));
	new->index = index;
	new->next = *l;
	*l = new;
}


MemLoc* list_pop(List** l){
	if(!(*l)) return NULL;
	List* temp = *l;
	MemLoc* output = &(temp->index);
	*l = temp->next;
	free(temp);
	return output;
}


void list_free(List** l){
	MemLoc* loc;
	while((loc = list_pop(l)) != NULL) /*free(loc)*/;
}


/**
 * 
 * 
 */


typedef struct GRAPH {
	MemArr* mem;
	List* context;
} Graph;

Graph** state(){
	static Graph* state = NULL;
	return &state;
}

MemArr** memory(){ return &((**state()).mem); }
List** context(){ return &((**state()).context); }


/**
 * 
 * 
 */


MemLoc gc_alloc(size_t size){
	MemLoc output = mem_arr_insert_data(*memory(), malloc(size));
	attach((**context()).index, output);
	return output;
}


void attach(MemLoc parent, MemLoc child){
	HashSet** h = mem_arr_get_hash(*memory(), parent);
	hash_set_insert(h, child);
}


void detach(MemLoc parent, MemLoc child){
	HashSet* h = *mem_arr_get_hash(*memory(), parent);
	hash_set_delete(h, child);
}


void* get(MemLoc index){
	return mem_arr_get_data(*memory(), index);
}


void start_garbage_collection(){
	*state() = calloc(sizeof(Graph), 1);
	*memory() = mem_arr_new();
	start_function();
}


void end_garbage_collection(){
	mem_arr_free(memory());
	list_free(context());
	free(*state());
}


void start_function(){
	list_push(context(), mem_arr_insert_data(*memory(), NULL));
}


void end_function(){
	list_pop(context());
}
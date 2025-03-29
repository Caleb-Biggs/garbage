#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "garbage.h"
#include "arena.h"
#include "node.h"


/**
 * Linked List
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


List* list_pop(List** l){
	if(!(*l)) return NULL;
	List* output = *l;
	*l = output->next;
	return output;
}


void list_free(List** l){
	List* node;
	while((node = list_pop(l)) != NULL) free(node);
}


/**
 * Global State
 */


typedef struct GRAPH {
	MemArr* mem;
	List* context;
	MemLoc root;
} Graph;

Graph** state(){
	static Graph* state = NULL;
	return &state;
}

MemArr** memory(){ return &((**state()).mem); }
List** context(){ return &((**state()).context); }
MemLoc* root(){ return &(**state()).root; }


/**
 * Garbage Collector
 **/


void depth_first(MemLoc root){
	if(mem_arr_is_marked(**memory(), root)) return;
	mem_arr_mark_keep(*memory(), root);
	Node n = *mem_arr_get_node(*memory(), root);
	node_for_each_neighbor(n, depth_first);
}


void collect_garbage(){
	// Do garbage collection if enough time has passed
	static uint16_t last_collected = 0;
	struct timespec time;
	clock_gettime(CLOCK_MONOTONIC, &time);
	uint64_t curr = time.tv_nsec + time.tv_sec*1e9;
	if(curr-last_collected < COLLECT_TIME) return;

	printf("Collecting Garbage\n");
	depth_first(*root());
	mem_arr_remove_unmarked(*memory());
	// mem_arr_print(*memory());
}


void print_node(MemLoc root){
	if(mem_arr_is_marked(**memory(), root)) return;
	printf("%lu ", root.x);
	mem_arr_mark_keep(*memory(), root);
	Node n = *mem_arr_get_node(*memory(), root);
	node_for_each_neighbor(n, print_node);
}


void print_graph(){
	printf("Total items in arena: %lu\n", (**memory()).size);
	print_node(*root());
	printf("\n");
	mem_arr_clear_marks(*memory());
	mem_arr_print(*memory());
}


/**
 * Public Interface
 */


MemLoc gc_alloc(size_t size){
	MemLoc output = mem_arr_insert_data(*memory(), malloc(size));
	attach((**context()).index, output);
	return output;
}


void attach(MemLoc parent, MemLoc child){
	Node* n = mem_arr_get_node(*memory(), parent);
	node_add_neighbor(n, child);
}


void detach(MemLoc parent, MemLoc child){
	Node* n = mem_arr_get_node(*memory(), parent);
	node_remove_neighbor(n, child);
}


void* get(MemLoc index){
	return mem_arr_get_data(*memory(), index);
}


void start_garbage_collection(){
	*state() = calloc(sizeof(Graph), 1);
	*memory() = mem_arr_new();
	start_function();
	*root() = (**context()).index;
}


void end_garbage_collection(){
	mem_arr_free(memory());
	list_free(context());
	free(*state());
}


void start_function(){
	MemLoc function = mem_arr_insert_data(*memory(), NULL);
	if(*context() != NULL)
		attach((**context()).index, function);
	list_push(context(), function);
	// collect_garbage();
}


MemLoc end_function(MemLoc* return_loc){
	List* l = list_pop(context()); //Might be NULL?
	MemLoc function = l->index;
	free(l);

	detach((**context()).index, function);
	if(return_loc == NULL) {
		collect_garbage();
		return (MemLoc){0};
	}
	attach((**context()).index, *return_loc);
	// collect_garbage();
	return *return_loc;
}
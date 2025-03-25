#include <stdlib.h>
#include "garbage.h"


GarbageCollector* garbage_collector_new(){
	GarbageCollector* output = malloc(sizeof(*output));
	output->memory = mem_arr_new();
	output->graph = calloc(sizeof(*(output->graph)), 1);
	gc_start_function(output);
	// output->graph->root = output->graph->context->n;
	return output;
}


void garbage_collector_free(GarbageCollector** gc){
	mem_arr_free(&((*gc)->memory));
	graph_free(&((*gc)->graph));
	free(*gc);
	*gc = NULL;
}


void gc_start_function(GarbageCollector* gc){
	graph_insert_function(gc->graph);
}


void* gc_end_function(GarbageCollector* gc, MemLoc* return_index){
	Node* n = list_pop(&(gc->graph->context));
	list_free(&(n->refs));
	free(n);
	// free(list_pop(&(gc->graph->context)));
	if(return_index == NULL) return NULL;
	graph_insert_data(gc->graph, *return_index);
	return gc->memory->data[return_index->v].d.p; // TODO: Error check
	return NULL; //TODO: Temporary
}


MemLoc gc_alloc(GarbageCollector* gc, size_t size){
	void* data = malloc(size);
	MemLoc output = mem_arr_insert(gc->memory, data);
	graph_insert_data(gc->graph, output);
	return output;
}
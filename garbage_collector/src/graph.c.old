#include <stdint.h>
#include <stdlib.h>
#include "graph.h"


// Assumes l is not NULL, *l can be NULL
void list_push(node_list** l, node* context){
	node_list* new = malloc(sizeof(node));
	new->n = context;
	new->next = *l;
	*l = new;
}


node* list_pop(node_list** l){
	if(!*l) return NULL;
	node_list* temp = *l;
	node* output = temp->n;
	*l = temp->next;
	free(temp);
	return output;
}


void list_free(node_list** l){
	while(list_pop(l) != NULL);
}


graph* get_graph(){
	static graph g = {
		.root = NULL,
		.context = NULL,
	};
	return &g;
}


void start_function(){
	node* new = malloc(sizeof(node));
	new->type = FUNCTION;
	new->num_refs = 0;
	new->refs = NULL;
	list_push(&(get_graph()->context), new);
}
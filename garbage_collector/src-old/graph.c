#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "graph.h"


Node* node_new(ContType type, MemLoc* index){
	Node* output = calloc(sizeof(*output), 1);
	// output->type = type;
	if(index) output->index = *index;
	return output;
}


// Assumes l is not NULL, *l can be NULL
void list_push(NodeList** l, Node* context){
	NodeList* new = malloc(sizeof(*new));
	new->n = context;
	new->next = *l;
	*l = new;
}


Node* list_pop(NodeList** l){
	if(!(*l)) return NULL;
	NodeList* temp = *l;
	Node* output = temp->n;
	*l = temp->next;
	free(temp);
	return output;
}


void list_free(NodeList** l){
	Node* n;
	while((n = list_pop(l)) != NULL) free(n);
}


void graph_insert_data(Graph* g, MemLoc index){
	list_push(&(g->context->n->refs), node_new(DATA, &index));
}


void graph_insert_function(Graph* g){
	Node* new = node_new(FUNCTION, NULL);
	if(g->root == NULL) g->root = new;
	else list_push(&(g->root->refs), new);
	list_push(&(g->context), new);
}


void graph_free_recursive(Node* n){
	if(!n) return;
	Node* curr;
	while((curr = list_pop(&(n->refs))) != NULL){
		graph_free_recursive(curr);
		// free(curr);
	}
	free(n);
}


void graph_free(Graph** g){
	// graph_free_recursive((*g)->root);
	list_free(&((*g)->root->refs));
	list_free(&((*g)->context));
	free(*g);
	*g = NULL;
}
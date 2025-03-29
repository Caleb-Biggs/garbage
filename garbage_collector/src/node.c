#include <stddef.h>
#include <stdlib.h>
#include "node.h"


Node* node_new(void* data){
	Node* node = calloc(sizeof(*node), 1);
	node->data = data;
	return node;
}


void* node_free(Node** n){
	void* data = (**n).data;
	if((**n).refs != NULL)
		hash_set_free(&((**n).refs));
	free(*n);
	*n = NULL;
	return data;
}


void* node_get_data(Node n){
	return n.data;
}


void node_for_each_neighbor(Node n, void(*func)(MemLoc)){
	if(n.refs == NULL) return;
	hash_set_for_each(*(n.refs), func);
}


void node_add_neighbor(Node* n, MemLoc neighbor){
	hash_set_insert(&(n->refs), neighbor);
}


void node_remove_neighbor(Node* n, MemLoc neighbor){
	hash_set_delete(n->refs, neighbor);
}
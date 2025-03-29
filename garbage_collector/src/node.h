#ifndef NODE_H
#define NODE_H

#include "shared.h"
#include "hashset.h"

typedef struct NODE {
	void* data;
	HashSet* refs;
} Node;

Node* node_new(void* data);
void* node_free(Node** n);
void* node_get_data(Node n);
void node_for_each_neighbor(Node n, void(*func)(MemLoc));
void node_add_neighbor(Node* n, MemLoc neighbor);
void node_remove_neighbor(Node* n, MemLoc neighbor);

#endif
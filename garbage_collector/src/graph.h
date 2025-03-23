#ifndef GRAPH_H
#define GRAPH_H

#include "arena.h"

typedef enum NODE_TYPE {
	MEMORY,	FUNCTION
} NodeType;

struct NODE_LIST;

typedef struct NODE {
	NodeType type;
	uint16_t num_refs;
	MemLoc index; // Not used for FUNCTION type
	struct NODE_LIST* refs;
} Node;

typedef struct NODE_LIST {
	Node* n;
	struct NODE_LIST* next;
} NodeList;

typedef struct GRAPH {
	Node* root;
	NodeList* context;
} Graph;

Node* node_new(NodeType type, MemLoc* index);

void list_push(NodeList** l, Node* context);
Node* list_pop(NodeList** l);
void list_free(NodeList** l);

void graph_free(Graph** g);
void graph_insert_data(Graph* g, MemLoc index);
void graph_insert_function(Graph* g);

#endif
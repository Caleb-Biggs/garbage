#ifndef GRAPH_H
#define GRAPH_H

typedef enum NODE_TYPE {
	// DATA, 
	FUNCTION
} node_type;

typedef struct NODE {
	node_type type;
	uint16_t num_refs;
	struct NODE* refs;
} node;

typedef struct NODE_LIST {
	node* n;
	struct NODE_LIST* next;
} node_list;

typedef struct GRAPH {
	node* root;
	node_list* context;
} graph;

void list_push(node_list** l, node* context);
node* list_pop(node_list** l);
void list_free(node_list** l);

#endif
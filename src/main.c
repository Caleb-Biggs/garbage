#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "garbage.h"


typedef struct NODE {
	int val;
	struct NODE* left;
	struct NODE* right;
} Node;
struct_setup(NODE, Node,
	type_memb(Node, left),
	type_memb(Node, right)
)

typedef struct TREE {
	Node* root;
} Tree;
struct_setup(TREE, Tree,
	type_memb(Tree, root)
)

Tree* tree_new(){
	return gc_alloc(TYPE_TREE());
}


void tree_insert_2(Tree* t, int val){
	if(!t) return;
	start_function();
	
	Node** n = &t->root;
	while(true){
		if(*n == NULL){
			*n = gc_alloc(TYPE_NODE());
			(**n).val = val;
			break;
		}
		if(val < (**n).val) n = &(**n).left;
		else if(val > (**n).val) n = &(**n).right;
		else break;
	}

	end_function(NULL);
}


void tree_insert_helper(Node** n, int val){
	if(!n) return;
	start_function();
	
	if(*n == NULL){
		*n = gc_alloc(TYPE_NODE());
		(**n).val = val;
	} else {
		if(val < (**n).val) tree_insert_helper(&(**n).left, val);
		else if(val > (**n).val) tree_insert_helper(&(**n).right, val);
	}

	end_function(NULL);
}

void tree_insert(Tree* t, int val){
	if(!t) return;
	tree_insert_helper(&t->root, val);
}

bool tree_contains_helper(Node* n, int val){
	if(!n) return false;
	else if(val < n->val) return tree_contains_helper(n->left, val);
	else if(val > n->val) return tree_contains_helper(n->right, val);
	else return true;
}

bool tree_contains(Tree* t, int val){
	if(!t) return false;
	return tree_contains_helper(t->root, val);
}


// #define TESTS 200000
#define TESTS 2000000
void tree_gc(){
	srand(time(NULL));
	start_garbage_collector();

	start_function();

		int vals[TESTS] = {0}; 
		Tree* t = tree_new();

		for(int i = 0; i < TESTS; i++){
			int r = rand()|1; // Only insert odd numbers
			vals[i] = r;
			tree_insert_2(t, r);
		}
		for(int i = 0; i < TESTS; i++){
			if(!tree_contains(t, vals[i])){
				printf("MISSING VALUE [%i]: %i\n", i, vals[i]);
			}
			if(tree_contains(t, vals[i]&(~1))){
				printf("INVALID VALUE [%i]: %i\n", i, vals[i]&(~1));
			}
		}

	end_function(NULL);

	usleep(1000000);
	graph_print_memory();
	
	end_garbage_collector();
}


int main(void){
	tree_gc();
}
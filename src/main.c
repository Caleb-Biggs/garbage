// #include "garbage.h" 
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "garbage.h"


typedef struct TREE {
	int val;
	struct TREE* left;
	struct TREE* right;
} Tree;
struct_setup(TREE, Tree,
	type_memb(Tree, left),
	type_memb(Tree, right)
)


// Tree* tree_gc_insert_iter(Tree** t, int val){
// 	if(!t) return;
// }


Tree* tree_gc_insert(Tree** t, int val){
	if(!t) return NULL;
	start_function();
	
	if(*t == NULL){
		// printf("NULL\n");
		*t = gc_alloc(TYPE_TREE());
		// printf("Tree: %p\n", (void*)(*t));
		(**t).val = val;
		end_function(*t);
		return *t;
	}

	// printf("T %p\n", (void*)*t);
	Tree* new = NULL;
	if(val < (**t).val){
		new = tree_gc_insert(&(**t).left, val);
	} else if(val > (**t).val){
		new = tree_gc_insert(&(**t).right, val);
	}

	end_function(new);
	return new;
}


void tree_gc(){
	start_garbage_collector();

	start_function();
		const int tests = 12000;
		// const int tests = 10;
		Tree* t = NULL;
		for(int i = 0; i < tests; i++){
			tree_gc_insert(&t, i);
			// usleep(10);
		}
	end_function(NULL);

	usleep(1000000);
	// graph_print_memory();
	// run_garbage_collection(NULL);
	// printf("----------\n");
	graph_print_memory();
	
	end_garbage_collector();
}


void tree_manual(){

}


void garbage(){
	// printf("Pointer arr: %lu\n", TYPE_POINTER_ARR().index);
	start_garbage_collector();

	// *(char*)gc_alloc(TYPE_CHAR()) = 'C';
	// *(int*)gc_alloc(TYPE_INT()) = 5;
	// *(int64_t*)gc_alloc(TYPE_INT64()) = INT64_MIN;
	// *(int64_t*)gc_alloc(TYPE_INT64()) = INT64_MAX;
	// Array a = array_new(TYPE_INT16(), 7);
	
	// *(uint16_t*)array_get(a, 0) = 0;
	// *(uint16_t*)array_get(a, 1) = 1;
	// *(uint16_t*)array_get(a, 2) = 2;
	// *(uint16_t*)array_get(a, 3) = 3;
	// *(uint16_t*)array_get(a, 4) = 4;
	// *(uint16_t*)array_get(a, 5) = 5;
	// *(uint16_t*)array_get(a, 6) = 6;
	// array_resize(&a, 10);
	// for(size_t i = 0; i < a.len; i++){
	// 	printf("%i\n", *(uint16_t*)array_get(a, i));
	// }
	// array_resize(&a, 3);
	// for(size_t i = 0; i < a.len; i++){
	// 	printf("%i\n", *(uint16_t*)array_get(a, i));
	// }

	// Array b = array_new(TYPE_POINTER(), 3);
	// *(char**)array_get(b, 0) = gc_alloc(TYPE_CHAR());
	// **(char**)array_get(b, 0) = 'C';
	// *(char**)array_get(b, 1) = gc_alloc(TYPE_CHAR());
	// **(char**)array_get(b, 1) = 'A';
	// *(char**)array_get(b, 2) = gc_alloc(TYPE_CHAR());
	// **(char**)array_get(b, 2) = 'L';
	// // *(char**)array_get(b, 3) = gc_alloc(TYPE_CHAR());
	// // **(char**)array_get(b, 3) = 'E';

	start_function();
	start_function();
	start_function();
	start_function();
	char* c = gc_alloc(TYPE_CHAR());
	graph_print_memory();
	end_function(c);
	end_function(c);
	end_function(c);
	end_function(c);

	usleep(1000000);
	end_garbage_collector();
}


int main(void){
	// garbage();
	tree_gc();
	// tree_manual();
}
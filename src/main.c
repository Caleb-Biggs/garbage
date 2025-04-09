// #include "garbage.h" 
#include <stdio.h>
#include <stdint.h>
#include "types.h"
#include "arena.h"
#include "meta_arena.h"
#include "arena_manager.h"
#include "garbage.h"
#include "array.h"


typedef struct TREE {
	struct TREE* left;
	struct TREE* right;
} Tree;
struct_setup(TREE, Tree,
	type_memb(Tree, left),
	type_memb(Tree, right)
)


void type_info(){
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
	run_garbage_collection();
	end_function(c);
	end_function(c);
	end_function(c);
	end_function(c);

	graph_print_memory();


	end_garbage_collector();//0x4a552e8 //0x4a53ef0
}


// void garbage(){
// 	start_garbage_collection();


// 		start_function();

// 		char* str = "abcdefghijklmnopqrstuvwxyz";
// 		for(int i = 0; i < 100; i++){
// 			MemLoc l = gc_alloc(sizeof(char));
// 			*(char*)get(l) = str[i%26];
// 		}

// 		MemLoc arr = gc_alloc(sizeof(int)*10);
// 		for(int i = 0; i < 10; i++){
// 			((int*)get(arr))[i] = i*i;
// 		}

// 		print_graph();
// 		collect_garbage();
// 		print_graph();

// 		// attach(l1, l2);
// 		// print_graph();
// 		// detach(l1, l2);

// 		end_function(&arr);
	
// 	collect_garbage();
// 	print_graph();
// 	// MemLoc l1 = gc_alloc(sizeof(char));
// 	// *(char*)get(l1) = 'C';
// 	// print_graph();


// 	end_garbage_collection();
// }


int main(void){
	// garbage();
	type_info();
}
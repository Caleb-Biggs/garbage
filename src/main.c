// #include "garbage.h" 
#include <stdio.h>
#include "types.h"
#include "memory.h"


typedef struct TREE_A {
	char* leaf_1;
	char* leaf_2;
} TreeA;
type_setup(TREE_A, TreeA,
	type_memb(TreeA, leaf_1),
	type_memb(TreeA, leaf_2)
)


void type_info(){
	

	Arena* a = arena_new(sizeof(int));
	Arena* b = arena_new(sizeof(char));

	size_t o; size_t* output = &o;
	for(int i = 0; i < 10; i++){
		if(!arena_insert(a, &output)) break;
		*(int*)arena_get_data(a, *output) = i;
	}

	// arena_delete(a, 0);
	// arena_delete(a, 1);
	// arena_delete(a, 2);
	// arena_delete(a, 3);

	for(int i = 10; i < 20; i++){
		if(!arena_insert(a, &output)) break;
		*(int*)arena_get_data(a, *output) = i;
	}

	arena_delete(a, 5);
	arena_delete(a, 7);
	arena_delete(a, 9);
	arena_delete(a, 11);


	arena_print(a);

	arena_free(&a);
	arena_free(&b);
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
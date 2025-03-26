#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "hashset.h"
#include "arena.h"


#define insert(a, x, y) char* (x) = malloc(sizeof(char)); *(x) = (y); mem_arr_insert_data((a), (x)); 


int main(int argc, char const* argv[]){
	// HashSet* h = NULL;
	// hash_set_insert(&h, (MemLoc){25});
	// hash_set_delete(h, (MemLoc){25});
	// hash_set_insert(&h, (MemLoc){17});
	// hash_set_insert(&h, (MemLoc){49});
	// hash_set_insert(&h, (MemLoc){96});
	// hash_set_insert(&h, (MemLoc){65});
	// hash_set_insert(&h, (MemLoc){87});
	// hash_set_insert(&h, (MemLoc){98});
	// hash_set_insert(&h, (MemLoc){13});
	// hash_set_print(*h);

	// if(hash_set_contains(*h, (MemLoc){25})) printf("Contains num\n");

	// hash_set_free(&h);


	MemArr* a = mem_arr_new();

	char* str = "CALEBBIGGS";
	for(int i = 0; i < 10; i++){
		insert(a, c, str[i]);
	}
	mem_arr_print(a);

	mem_arr_mark_delete(a, (MemLoc){0});
	mem_arr_mark_delete(a, (MemLoc){2});
	mem_arr_mark_delete(a, (MemLoc){3});
	mem_arr_mark_delete(a, (MemLoc){5});
	mem_arr_mark_delete(a, (MemLoc){7});
	mem_arr_remove_marked(a);
	mem_arr_print(a);

	char* str2 = "abcdefghijklmnopqrstuvwxyz";
	for(int i = 0; i < 3; i++){
		insert(a, c, str2[i]);
	}
	mem_arr_print(a);

	mem_arr_free(&a);
}
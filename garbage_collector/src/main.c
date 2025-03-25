#include <stddef.h>
#include <stdio.h>
#include "hashset.h"


int main(int argc, char const* argv[]){
	HashSet* h = NULL;
	hash_set_insert(&h, (MemLoc){25});
	hash_set_delete(h, (MemLoc){25});
	hash_set_insert(&h, (MemLoc){17});
	hash_set_insert(&h, (MemLoc){49});
	hash_set_insert(&h, (MemLoc){96});
	hash_set_insert(&h, (MemLoc){65});
	hash_set_insert(&h, (MemLoc){87});
	hash_set_insert(&h, (MemLoc){98});
	hash_set_insert(&h, (MemLoc){13});
	hash_set_print(*h);

	if(hash_set_contains(*h, (MemLoc){25})) printf("Contains num\n");

	hash_set_free(&h);
}
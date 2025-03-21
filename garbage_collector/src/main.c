#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <valgrind/memcheck.h>


const size_t ARENA_MAX = 8;

typedef struct ARENA {
	size_t data_size;
	uint32_t arena_size;
	uint32_t arena_max;
	uint8_t last_empty;
	uint8_t* data;
} arena;


arena* arena_new(size_t data_size){
	arena* output = malloc(sizeof(arena));
	output->data_size = data_size;
	output->arena_size = 0;
	output->arena_max = ARENA_MAX;
	output->last_empty = ARENA_MAX-1;
	output->data = malloc(data_size * ARENA_MAX); // TODO: This can be malloc
	for(uint8_t i = 0; i < ARENA_MAX; i++){
		output->data[data_size*i] = i-1;
	}
	return output;
}


void arena_free(arena** a){
	free((*a)->data);
	free(*a);
	*a = NULL;
}


void* new(arena* a){
	if(a->arena_size == ARENA_MAX) return NULL;
	a->arena_size++;
	uint32_t index = a->data_size*a->last_empty;
	void* output = a->data+index;
	a->last_empty = a->data[index];
	return output;
}


void rem(arena* a, void* d_void){
	uint8_t* d = (uint8_t*)d_void;
	if(d < a->data || d >= a->data+(a->arena_max*a->data_size)) return;
	if(a->arena_size != ARENA_MAX) *d = a->last_empty;
	a->last_empty = (d-(a->data))/(a->data_size);
	a->arena_size--;
}


// Calling this will disable subsequent Valgrind errors regarding uninitialized
// values, so only call after other functions you want to error check
void arena_print(arena a){
	VALGRIND_MAKE_MEM_DEFINED(a.data, a.arena_max*a.data_size);
	for(int i = 0; i < ARENA_MAX*a.data_size; i++){
		printf("~%c~%i~\n", a.data[i], a.data[i]);
	}
}


#define TYPE uint16_t
int main(int argc, char const *argv[]){
	arena* a = arena_new(sizeof(TYPE));

	TYPE* n0 = new(a);
	*n0 = (uint16_t)'C'<<8|'A';

	TYPE* n1 = new(a);
	*n1 = (uint16_t)'L'<<8|'E';

	TYPE* n2 = new(a);
	*n2 = (uint16_t)'B'<<8|'B';

	TYPE* n3 = new(a);
	*n3 = (uint16_t)'I'<<8|'G';

	TYPE* n4 = new(a);
	*n4 = (uint16_t)'G'<<8|'S';

	TYPE* n5 = new(a);
	*n5 = (uint16_t)'P'<<8|'I';

	TYPE* n6 = new(a);
	*n6 = (uint16_t)'Z'<<8|'Z';

	TYPE* n7 = new(a);
	*n7 = (uint16_t)'A'<<8|'1';

	rem(a, n3);
	rem(a, n5);

	TYPE* n8 = new(a);
	*n8 = (uint16_t)'A'<<8|'B';

	TYPE* n9 = new(a);
	*n9 = (uint16_t)'C'<<8|'D';	

	arena_print(*a);
	arena_free(&a);

	return 0;
}
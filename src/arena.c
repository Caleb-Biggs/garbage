#include <stdio.h>
#include <string.h>
#include "arena.h"
#include "types.h"


size_t arena_bytes(size_t data_size){
	return (sizeof(Metadata)+data_size);
}


int arena_new(size_t data_size, Arena* output){
	if(!output) return -1;
	void* data = calloc(ARENA_SIZE, arena_bytes(data_size));
	if(!data) return -2;
	*output = (Arena){
		.num_items = 0,
		.empty = 0,
		.last = 0,
		.data_size = data_size,
		.data = data
	};
	return 0;
}

void arena_free(Arena* a){
	if(!a) return;
	free(a->data);
	a->data = NULL;
}


// Sets output to NULL upon error, new index on success
bool arena_insert(Arena* a, size_t** output, TypeIndex t){
	if(!a || a->num_items >= ARENA_SIZE)
		return (*output = NULL);
	// TODO: error check output

	**output = a->empty;
	Metadata* m = arena_get_metadata(a, **output);
	if(a->last == a->empty) a->empty = ++a->last;
	else a->empty = m->index;
	a->num_items++;

	*m = (Metadata){.label = DATA, .type = t};
	return true;
}


void arena_mark_for_deletion(Arena* a, size_t index){
	Metadata* m = arena_get_metadata(a, index);
	if(m->label != DATA) return; // TODO: Return Error?
	m->mark = true;
}


void arena_delete_unmarked(Arena* a){
	for(size_t i = 0; i < a->last; i++){
		Metadata* m = arena_get_metadata(a, i);
		if(m->label != DATA) continue;
		if(m->mark){
			m->mark = false;
			continue;
		}
		
		*m = (Metadata){
			.mark = false, 
			.label = INDEX, 
			.index = a->empty
		};
		memset(m+1, 0, a->data_size);
		a->empty = i;
		a->num_items--;
	}
}


void* arena_get_data(Arena* a, size_t index){
	// TODO: Error check?
	return arena_get_metadata(a, index)+1;
}


Metadata* arena_get_metadata(Arena* a, size_t index){
	//TODO: Error check?
	return (Metadata*)((uint8_t*)(a->data) + (index*arena_bytes(a->data_size)));
}


bool arena_is_full(Arena a){
	return (a.num_items >= ARENA_SIZE);
}


bool arena_is_empty(Arena a){
	return (a.num_items == 0);
}


void arena_print(Arena* a){
	printf("Filled: %lu / %u\n", a->num_items, ARENA_SIZE);
	printf("Next Empty: %lu\n", a->empty);
	printf("Last: %lu\n", a->last);
	printf("Data size: %lu\n", a->data_size);
	// for(int i = 0; i < a->last; i++){
	// 	Metadata* m = arena_get_metadata(a, i);
	// 	if(m->label == DATA){
	// 		printf("Label: DATA\n");
	// 		printf("Mark: %i\n", m->mark);
	// 		printf("Type: %lu\n", m->type.index);
	// 		printf("Pointer: %p\n", arena_get_data(a, i));
	// 	} else {
	// 		// printf("Label: INDEX\n");
	// 		// printf("Mark: %i\n", m->mark);
	// 		// printf("Index: %lu\n", m->index);
	// 	}
	// }
}
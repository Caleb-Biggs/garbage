#include <stdio.h>
#include "memory.h"


Arena* arena_new(size_t data_size){
	Arena* output = malloc(sizeof(*output));
	if(output == NULL) return NULL;

	output->max_items = ARENA_SIZE;
	output->num_items = 0;
	output->empty = 0;
	output->last = 0;
	output->data_size = data_size;
	if(!(output->metadata = malloc(ARENA_SIZE * sizeof(*(output->metadata))))
		|| !(output->data = calloc(ARENA_SIZE, data_size))
	) return NULL;

	return output;
}


void arena_free(Arena** a){
	if(!a || !*a) return;
	free((**a).metadata);
	free((**a).data);
	free(*a);
	*a = NULL;
}


// Sets output to NULL upon error, new index on success
bool arena_insert(Arena* a, size_t** output){
	if(!a || a->num_items >= a->max_items)
		return (*output = NULL);

	**output = a->empty;
	if(a->last == a->empty) a->empty = ++a->last;
	else a->empty = a->metadata[**output].index;
	a->num_items++;

	Metadata* m = arena_get_metadata(a, **output);
	*m = (Metadata){.label=DATA, .type={0}}; // TODO: type info
	return true;
}


bool arena_delete(Arena* a, size_t index){
	if(!a || index >= a->last) return false;
	// TODO: ensure index points to data

	Metadata* m = arena_get_metadata(a, index);
	*m = (Metadata){.label=INDEX, .index=a->empty};
	a->empty = index;
	a->num_items--;
	return true;
}


Metadata* arena_get_metadata(Arena* a, size_t index){
	//TODO: Error check?
	return a->metadata+index;
}


void* arena_get_data(Arena* a, size_t index){
	// TODO: Error check?
	return (uint8_t*)(a->data)+(index*a->data_size);
}


int arena_index_from_pointer(Arena* a, void* pointer, size_t** output){
	size_t offset;
	int err = 0;
	if(!a) err = -1;
	else if(!output) return -2;
	else if(!(*output)) err = -3;
	else if(pointer < a->data) err = -4;
	else if(pointer > (void*)((uint8_t*)(a->data)+(a->data_size*(a->max_items-1))))
		err = -5;
	else if((offset = (size_t)pointer - (size_t)a->data) % a->data_size != 0)
		err = -6;
	
	if(err < 0){
		*output = NULL;
		return err;
	}

	offset /= a->data_size;
	**output = offset;
	return err;
}


void arena_print(Arena* a){
	printf("Filled: %lu / %lu\n", a->num_items, a->max_items);
	printf("Next Empty: %lu\n", a->empty);
	printf("Last: %lu\n", a->last);
	printf("Data size: %lu\n", a->data_size);
	for(int i = 0; i < a->last; i++){
		Metadata* m = arena_get_metadata(a, i);
		if(m->label == DATA){
			printf("Label: DATA\n");
			printf("Type: %lu\n", m->type.index);
			if(a->data_size == 1)
				printf("Data: %c\n", *(char*)arena_get_data(a, i));
			else if(a->data_size == 4)
				printf("Data: %i\n", *(int*)arena_get_data(a, i));
		} else {
			printf("Label: INDEX\n");
			printf("Type: %lu\n", m->index);
		}
	}
}
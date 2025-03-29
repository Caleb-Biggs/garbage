#include <stdlib.h>
#include <stdio.h>
#include "hashset.h"


HashSet* hash_set_init(uint16_t size){
	HashSet* output = malloc(sizeof(*output));
	output->num_buckets = size;
	output->size = 0;
	output->data = calloc(sizeof(*(output->data)), output->num_buckets);
	if((output->is_full = bit_field_new(size)) == NULL) return NULL;
	if(!output->is_full) return NULL;
	return output;
}


void hash_set_resize(HashSet** h){
	if(h == NULL || *h == NULL) return; //TODO: better error?
	HashSet* output = hash_set_init((**h).num_buckets * HASH_SET_INCREASE);
	for(uint16_t i = 0; i < (**h).num_buckets; i++){
		int is_full = bit_field_get(*(**h).is_full, i);
		if(is_full == -1); // TODO: Error
		if(is_full == 1) hash_set_insert(&output, (**h).data[i]);
	}
	hash_set_free(h);
	*h = output;
}


uint64_t hash_function(MemLoc data){
	return data.x;
}


// TODO: Comments!
#define SIZE_ERROR INT32_MIN
int32_t hash_set_find(HashSet h, MemLoc data){
	if(h.size >= h.num_buckets) return SIZE_ERROR;
	int32_t start = hash_function(data) % h.num_buckets;
	while(true){
		if(data.x == h.data[start].x) return -start; // Data already exists
		int is_full = bit_field_get(*h.is_full, start);
		if(is_full == -1); // TODO: Error
		if(is_full == 0) return start; //0 or 1?
		if(start++ == h.num_buckets) start = 0;
	}
}


void hash_set_insert(HashSet** h, MemLoc data){
	if(h == NULL) return;
	if(*h == NULL) *h = hash_set_init(HASH_SET_MIN);
	if((**h).size >= (uint16_t)((**h).num_buckets*HASH_SET_RATIO))
		hash_set_resize(h);

	int32_t insert_at = hash_set_find(**h, data);
	if(insert_at < 0) printf("hash_set_find failed");
	(**h).data[insert_at] = data;
	if(bit_field_set((**h).is_full, insert_at, true) == -1); // TODO: Error
	(**h).size++;
}


bool hash_set_contains(HashSet h, MemLoc data){
	int32_t loc = hash_set_find(h, data);
	return (loc < 0 && loc != SIZE_ERROR);
}


void hash_set_delete(HashSet* h, MemLoc data){
	int32_t loc = hash_set_find(*h, data);
	if(loc >= 0 || loc == SIZE_ERROR) return; //TODO: Return error?
	else loc = -loc;
	h->size--;
	if(bit_field_set(h->is_full, loc, false) == -1); //TODO: Error
}


void hash_set_print(HashSet h){
	printf("Max size = %i; Filled = %i\n", h.num_buckets, h.size);
	for(uint16_t i = 0; i < h.num_buckets; i++){
		int is_full = bit_field_get(*h.is_full, i);
		char full_char = (is_full == -1) ? 'E' : (is_full == 0) ? 'e' : 'f';
		printf("[%i] %li %c\n", i, h.data[i].x, full_char);
	}
}


void hash_set_free(HashSet** h){
	free((**h).data);
	bit_field_free((**h).is_full);
	free(*h);
	*h = NULL;
}


void hash_set_for_each(HashSet h, void(*func)(MemLoc)){
	for(uint16_t i = 0; i < h.num_buckets; i++){
		int is_full = bit_field_get(*h.is_full, i);
		if(is_full == -1); // TODO: error
		if(is_full == 1) func(h.data[i]);
	}
}
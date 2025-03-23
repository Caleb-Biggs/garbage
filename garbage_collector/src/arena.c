#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "test.h"


uint64_t marks_size(MemArr* a){
	return (uint64_t)(ceilf((double)(a->max_size)/(8.0*sizeof(*(a->marks)))));
}


bool is_marked(MemArr* a, uint64_t index){
	// TODO: Check index OOB?
	// if(index > a->max_size);
	size_t sz = sizeof(*(a->marks))*8;
	return a->marks[index/sz] >> (index%sz) & 1;
}


void mark_keep(MemArr* a, uint64_t index){
	size_t sz = sizeof(*(a->marks))*8;
	a->marks[index/sz] |= (((uint64_t)1)<<(index%sz));
}


MemArr* mem_arr_new(){
	MemArr* output = malloc(sizeof(*output));
	output->empty = 0;
	output->last = 0;
	output->size = 0;
	output->max_size = MEM_ARR_SIZE;
	output->marks = calloc(marks_size(output), sizeof(*(output->marks)));
	output->data = malloc(output->max_size * sizeof(*(output->data)));
	return output;
}


void mem_arr_free(MemArr** a){
	if(a == NULL) return;
	free((*a)->marks);
	for(uint64_t i = 0; i < (*a)->last; i++){
		if((*a)->data[i].type != DATA) continue;
		// free((*a)->data[i].data.p); // TODO: uncomment
	}
	free((*a)->data);
	free(*a);
	*a = NULL;
}


bool mem_arr_resize(MemArr* a){
	if(a == NULL) return false;
	uint64_t prev_size = marks_size(a);
	a->max_size *= RESIZE_RATIO;
	printf("Realloc %lu\n", marks_size(a) * sizeof(*(a->marks)));
	a->marks = realloc(a->marks, marks_size(a) * sizeof(*(a->marks)));
	memset(a->marks+prev_size, 0, prev_size);
	a->data = realloc(a->data, a->max_size * sizeof(*(a->data)));
	return true;
}


MemCont data_container(void* data){
	return (MemCont){
		.type = DATA,
		.data = { .p = data }
	};
}


MemCont index_container(uint64_t index){
	return (MemCont){
		.type = INDEX,
		.data = { .i = index }
	};
}


bool mem_arr_push(MemArr* a, void* data){
	if(a == NULL) return false;
	if(a->size == a->max_size) mem_arr_resize(a);
	a->data[a->size] = data_container(data);
	a->empty++;
	a->last++;
	a->size++;
	return true;
}


bool mem_arr_insert(MemArr* a, void* data){
	if(a == NULL) return false;
	if(a->last == a->empty) return mem_arr_push(a, data);
	uint64_t index = a->empty;
	a->empty = a->data[index].data.i;
	a->data[index] = data_container(data);
	a->size++;
	return true;
}


void mem_arr_remove_marked(MemArr* a){
	for(uint64_t i = 0; i < a->last; i++){
		if(is_marked(a, i)) continue;
		a->data[i] = index_container(a->empty);
		a->empty = i;
		a->size--;
	}
	memset(a->marks, 0, marks_size(a));
}


void mem_arr_print(MemArr* a){
	printf("Size = %li; Max = %li; Empty = %li\n", a->last, a->max_size, a->empty);
	for(int i = 0; i < a->last; i++){
		if(a->data[i].type == INDEX) 
			printf("[%i] %i; i: %li\n", i, is_marked(a, i), a->data[i].data.i);
		else printf("[%i] %i; d: %c\n", i, is_marked(a, i), *(char*)(a->data[i].data.p));
	}
	for(int i = 0; i < marks_size(a); i++){
		printf("%lo\n", (uint64_t)(a->marks[i]));
	}
}
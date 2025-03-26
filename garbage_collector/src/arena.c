#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "arena.h"


MemArr* mem_arr_new(){
	MemArr* output = malloc(sizeof(*output));
	output->empty = 0;
	output->last = 0;
	output->size = 0;
	output->max_size = MEM_ARR_SIZE;
	output->marks = bit_field_new(output->max_size);
	output->data = malloc(output->max_size * sizeof(*(output->data)));
	return output;
}


void mem_cont_free(MemArr* a, MemLoc index){
	if(a->data[index.x].type != DATA) return;
	if(a->data[index.x].d.p->refs != NULL){
		hash_set_free(&(a->data[index.x].d.p->refs));
	}
	free(mem_arr_get_data(a, index));
	free(a->data[index.x].d.p);
}


void mem_arr_free(MemArr** a){
	if(a == NULL) return;
	bit_field_free((*a)->marks);
	for(uint64_t i = 0; i < (*a)->last; i++){
		mem_cont_free(*a, (MemLoc){i});
	}
	free((*a)->data);
	free(*a);
	*a = NULL;
}


void* mem_arr_get_data(MemArr* a, MemLoc index){
	return a->data[index.x].d.p->data;
}


Node* mem_arr_get_node(MemArr* a, MemLoc index){
	return a->data[index.x].d.p;
}


HashSet** mem_arr_get_hash(MemArr* a, MemLoc index){
	return &(a->data[index.x].d.p->refs);
}


bool mem_arr_resize(MemArr* a){
	if(a == NULL) return false;
	a->max_size *= RESIZE_RATIO;
	free(a->marks.bits);
	a->marks = bit_field_new(a->max_size);
	a->data = realloc(a->data, a->max_size * sizeof(*(a->data)));
	return true;
}


MemCont data_container(void* data){
	Node* node = calloc(sizeof(*node), 1);
	node->data = data;
	return (MemCont){
		.type = DATA,
		.d = { .p = node }
	};
}


MemCont index_container(uint64_t index){
	return (MemCont){
		.type = INDEX,
		.d = { .i = index }
	};
}


MemLoc mem_arr_push(MemArr* a, MemCont m){
	if(a == NULL) printf("mem_arr_push should not recieve NULL");
	if(a->size == a->max_size) mem_arr_resize(a);
	MemLoc index = {a->empty++};
	a->data[a->size] = m;
	a->last++;
	a->size++;
	return index;
}


MemLoc mem_arr_insert(MemArr* a, MemCont m){
	if(a == NULL) printf("mem_arr_insert should not recieve NULL");
	if(a->last == a->empty) return mem_arr_push(a, m);
	MemLoc index = {a->empty};
	a->empty = a->data[index.x].d.i;
	a->data[index.x] = m;
	a->size++;
	return index;
}


MemLoc mem_arr_insert_data(MemArr* a, void* data){
	return mem_arr_insert(a, data_container(data));
}


void mem_arr_mark_keep(MemArr* a, MemLoc index){
	bit_field_set(&(a->marks), index, true);
}


void mem_arr_remove_unmarked(MemArr* a){
	for(uint64_t i = 0; i < a->last; i++){
		if(bit_field_get(a->marks, (MemLoc){i}) == 1) continue;
		mem_cont_free(a, (MemLoc){i});
		a->data[i] = index_container(a->empty);
		a->empty = i;
		a->size--;
	}
	mem_arr_clear_marks(a);
}


void mem_arr_clear_marks(MemArr* a){
	memset(a->marks.bits, 0, bit_field_size(a->max_size));
}


void mem_arr_print(MemArr* a){
	printf("Size = %li; Max = %li; Empty = %li\n", a->last, a->max_size, a->empty);
	for(int i = 0; i < a->last; i++){
		if(a->data[i].type == INDEX) 
			printf("[%i] %i; i: %li\n", i, bit_field_get(a->marks, (MemLoc){i}), a->data[i].d.i);
		else if(mem_arr_get_data(a, (MemLoc){i}) == NULL){
			printf("[%i] %i; d: %p\n", i, bit_field_get(a->marks, (MemLoc){i}), mem_arr_get_data(a, (MemLoc){i}));
		} else printf("[%i] %i; d: %c\n", i, bit_field_get(a->marks, (MemLoc){i}), *(char*)(mem_arr_get_data(a, (MemLoc){i})));
	}
}
#ifndef ARENA_H
#define ARENA_H

#include "bitfield.h"
#include "hashset.h"
#include "shared.h"

#define MEM_ARR_SIZE 8
#define RESIZE_RATIO 2

typedef struct NODE {
	void* data;
	HashSet* refs;
} Node;

typedef enum CONTAINER_TYPE {
	DATA, INDEX
} ContType;

typedef union CONTAINER_DATA {
	Node* p;
	uint64_t i;
} ContData;

typedef struct MEMORY_CONTAINER {
	ContType type;
	ContData d;
} MemCont;

typedef struct MEMORY_ARRAY {
	uint64_t empty;
	uint64_t last;
	uint64_t size;
	uint64_t max_size;
	BitField marks;
	MemCont* data;
} MemArr;

MemArr* mem_arr_new();
void mem_arr_free(MemArr** a);
void* mem_arr_get_data(MemArr* a, MemLoc index);
Node* mem_arr_get_node(MemArr* a, MemLoc index);
MemLoc mem_arr_insert_data(MemArr* a, void* data);
void mark_keep(MemArr* a, MemLoc index);
void mem_arr_mark_delete(MemArr* a, MemLoc index);
void mem_arr_remove_marked(MemArr* a);
void mem_arr_print(MemArr* a);
HashSet** mem_arr_get_hash(MemArr* a, MemLoc index);

#endif
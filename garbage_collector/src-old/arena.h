#ifndef ARENA_H
#define ARENA_H

#include <stdint.h>
#include <stdbool.h>

#define MEM_ARR_SIZE 8
#define RESIZE_RATIO 2


typedef struct MEMORY_LOCATION { uint64_t v; } MemLoc;

typedef struct NODE {
	uint16_t num_refs;
	void* data;
	MemLoc index; // Not used for FUNCTION type
	struct NODE_LIST* refs;
} Node;

typedef struct NODE_LIST {
	Node* n;
	struct NODE_LIST* next;
} NodeList;

typedef enum CONTAINER_TYPE {
	DATA, INDEX, FUNCTION
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
	uint64_t* marks; //TODO: Change back to uint_64_t
	MemCont* data;
} MemArr;


MemArr* mem_arr_new();
void mem_arr_free(MemArr** a);
MemLoc mem_arr_insert(MemArr* a, void* data);
void mark_keep(MemArr* a, MemLoc index);
void mem_arr_remove_marked(MemArr* a);
void mem_arr_print(MemArr* a);

#endif
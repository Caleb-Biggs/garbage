#ifndef ARENA_H
#define ARENA_H

#include <stdint.h>
#include <stdbool.h>

#define MEM_ARR_SIZE 8
#define RESIZE_RATIO 2

typedef enum CONTAINER_TYPE {
	DATA, INDEX
} ContType;

typedef union CONTAINER_DATA {
	void* p;
	uint64_t i;
} ContData;

typedef struct MEMORY_CONTAINER {
	ContType type;
	ContData data;
} MemCont;

typedef struct MEMORY_ARRAY {
	uint64_t empty;
	uint64_t last;
	uint64_t size;
	uint64_t max_size;
	uint64_t* marks; //TODO: Change back to uint_64_t
	MemCont* data;
} MemArr;

typedef struct MEMORY_LOCATION { uint64_t v; } MemLoc;


MemArr* mem_arr_new();
void mem_arr_free(MemArr** a);
MemLoc mem_arr_insert(MemArr* a, void* data);
void mark_keep(MemArr* a, MemLoc index);
void mem_arr_remove_marked(MemArr* a);
void mem_arr_print(MemArr* a);

#endif
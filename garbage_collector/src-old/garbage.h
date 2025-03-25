#ifndef GARBAGE_H
#define GARBAGE_H

#include "arena.h"
#include "graph.h"

typedef struct GARBAGE_COLLECTOR {
	MemArr* memory;
	Graph* graph;
} GarbageCollector;

GarbageCollector* garbage_collector_new();
void garbage_collector_free(GarbageCollector** gc);
void gc_start_function(GarbageCollector* gc);
void* gc_end_function(GarbageCollector* gc, MemLoc* return_index);
MemLoc gc_alloc(GarbageCollector* gc, size_t size);

#endif
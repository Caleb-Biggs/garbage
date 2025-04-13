#ifndef GARBAGE_H
#define GARBAGE_H

#include "types.h"

int start_garbage_collector();
void end_garbage_collector();
void start_function();
void end_function(void* ret);
void* gc_alloc(TypeIndex t);
// void* gc_alloc_array(TypeIndex t, size_t num);
// void gc_realloc_array(Array* a, size_t num);

// Debug
void graph_print_memory();
void graph_print();
void* run_garbage_collection(void* _);

#endif
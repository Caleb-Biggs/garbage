#ifndef GARBAGE_H
#define GARBAGE_H

#include <stddef.h>
#include "shared.h"

MemLoc gc_alloc(size_t size);
void attach(MemLoc parent, MemLoc child);
void detach(MemLoc parent, MemLoc child);
void* get(MemLoc index);

void start_garbage_collection();
void end_garbage_collection();
void start_function();
void end_function();

#endif
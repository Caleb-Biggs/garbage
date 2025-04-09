#ifndef ARENA_MANAGER_H
#define ARENA_MANAGER_H

#include "meta_arena.h"

#define STARTING_META_ARENAS 8

typedef struct ARENA_MANAGER {
	size_t largest;
	MetaArena* arena_size;
} ArenaManager;

int manager_new(ArenaManager* output);
void manager_free(ArenaManager* a);
void* manager_allocate(ArenaManager* a, TypeIndex t);
void manager_delete_unmarked(ArenaManager a);

// Debug
void manager_print(ArenaManager a);

#endif
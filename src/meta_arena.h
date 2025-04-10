#ifndef META_ARENA_H
#define META_ARENA_H

#include "arena.h"

#define STARTING_ARENAS 0
#define META_ARENA_RESIZE 1.5

// TODO: data_size may be able to be removed to save space
// TODO: can fill_next and num_arenas be reduced to 32 bit ints?
typedef struct META_ARENA {
	size_t fill_next;		// The next arena to fill (should always be the first empty)
	size_t num_arenas;		// How many arenas are in the list
	size_t max_arenas;
	size_t data_size;		// Size of the data being stored
	size_t* arena_next;		// A list of indices pointing to the next arena to fill
	Arena* arenas;			// List of arenas
} MetaArena;

// Public methods
int meta_arena_new(size_t data_size, MetaArena* output);
void meta_arena_free(MetaArena* m);
void* meta_arena_allocate(MetaArena* m, TypeIndex t);
void meta_arena_delete_unmarked(MetaArena* m);

// Debug
void meta_arena_mark_delete(MetaArena* m, size_t arena, size_t index);
void meta_arena_print(MetaArena m);

#endif
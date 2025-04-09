#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include <stdint.h>
#include "metadata.h"

// TODO: num_items, empty, and last only need to be large enough to hold ARENA_SIZE
// TODO: data_size probably has a reasonable max size that's less than 64 bits
typedef struct ARENA {
	size_t num_items;	// How many items are currently in the arena
	size_t empty;		// The next empty index to fill
	size_t last;		// The highest index that could be filled
	size_t data_size;	// The size of the data being stored
	void* data;			// The memory, with metadata before each chunk
} Arena;

#define ARENA_SIZE 64

// Public
int arena_new(size_t data_size, Arena* output);
void arena_free(Arena* a);
bool arena_insert(Arena* a, size_t** output, TypeIndex t);
void arena_delete_unmarked(Arena* a);
void* arena_get_data(Arena* a, size_t index);
Metadata* arena_get_metadata(Arena* a, size_t index);
bool arena_is_full(Arena a);
bool arena_is_empty(Arena a);

// Debug
void arena_print(Arena* a);
void arena_mark_for_deletion(Arena* a, size_t index);

#endif
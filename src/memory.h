#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include <stdint.h>
#include "types.h"

typedef struct METADATA {
	enum { DATA, INDEX
	} label;
	union {
		TypeIndex type;
		size_t index;
	};
} Metadata;

typedef struct ARENA {
	size_t max_items;	// How many items can be put into arena
	size_t num_items;	// How many items are currently in the arena
	size_t empty;		// The next empty index to fill
	size_t last;		// The highest index that could be filled
	size_t data_size;	// The size of the data being stored
	Metadata* metadata;	// Metadata for each piece of data
	void* data;			// The memory
} Arena;

#define ARENA_SIZE 16

// Public
Arena* arena_new(size_t data_size);
void arena_free(Arena** a);
bool arena_insert(Arena* a, size_t** output);
Metadata* arena_get_metadata(Arena* a, size_t index);
void* arena_get_data(Arena* a, size_t index);
bool arena_delete(Arena* a, size_t index);
int arena_index_from_pointer(Arena* a, void* pointer, size_t** output);

// Debug
void arena_print(Arena* a);

#endif
#ifndef META_ARENA_H
#define META_ARENA_H

#include "arena.h"

typedef struct META_ARENA {
	size_t max_size;
	struct {
		size_t fill_next;
		size_t num_arenas;
		Arena* arenas;
	} arena_size*;
} MetaArena;

#endif
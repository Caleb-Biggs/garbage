#ifndef HASH_SET_H
#define HASH_SET_H

// #include <stdint.h>
#include <stdbool.h>
#include "shared.h"

#define HASH_SET_MIN 2
#define HASH_SET_RATIO 0.7
#define HASH_SET_INCREASE 2


typedef struct HASH_SET {
	uint16_t num_buckets;
	uint16_t size;
	MemLoc* data;
	bool* is_full; // TODO: Replace with bitset
} HashSet;


void hash_set_insert(HashSet** h, MemLoc data);
void hash_set_delete(HashSet* h, MemLoc data);
bool hash_set_contains(HashSet h, MemLoc data);
void hash_set_print(HashSet h);
void hash_set_free(HashSet** h);


#endif
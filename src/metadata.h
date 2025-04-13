#ifndef METADATA_H
#define METADATA_H

#include "types.h"

// TODO: the mark and label fields could be combined into a single
// "flag" field, but would this actually save space in the struct or
// would padding be added anyway?
typedef struct METADATA {
	bool mark;
	// bool pointer; // Only used when label is CHUNK
	enum { DATA, INDEX//, CHUNK
	} label;
	union {
		TypeIndex type;	// DATA
		size_t index;	// INDEX
		// size_t size;	// CHUNK
	};
} Metadata;


Metadata* metadata_get(void* data);

#endif
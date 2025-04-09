#ifndef METADATA_H
#define METADATA_H

#include "types.h"

// TODO: the mark and label fields could be combined into a single
// "flag" field, but would this actually save space in the struct or
// would padding be added anyway?
typedef struct METADATA {
	bool mark;
	enum { DATA, INDEX
	} label;
	union {
		TypeIndex type;
		size_t index;
	};
} Metadata;


Metadata* metadata_get(void* data);
void metadata_mark_delete(void* data);

#endif
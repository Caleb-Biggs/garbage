#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include <stdlib.h>


typedef struct TYPE_INDEX {
	size_t index;
} TypeIndex;

// typedef TypeIndex (*TypeLabel)();

typedef struct TYPE_INFO {
	size_t struct_sz;
	size_t num_memb;
	void** members;
} TypeInfo;

#define TYPE_INIT_SIZE 4
#define TYPE_INIT_RATIO 2

TypeIndex type_init(size_t struct_sz, size_t num_memb, void** members);
TypeInfo* type_get_info(TypeIndex t);

#define type_memb(type, mem) (void*)(&((type*)0)->mem)

#define type_setup(NAME, type, ...) 					\
	TypeIndex INIT_##NAME(){							\
		void* members[] = { __VA_ARGS__ };				\
		return type_init(								\
			sizeof(type), 								\
			sizeof(members)/sizeof(*members), 			\
			members										\
		);												\
	}													\
														\
	TypeIndex TYPE_##NAME(){							\
		static bool is_initialized = false;				\
		static TypeIndex t;								\
		if(!is_initialized) t = INIT_##NAME();			\
		return t;										\
	}

#endif
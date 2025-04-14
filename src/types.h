#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#define TYPE_INIT_SIZE 4
#define TYPE_INIT_RATIO 2

typedef struct TYPE_INDEX {
	size_t index;
} TypeIndex;

typedef struct TYPE_INFO {
	size_t struct_sz;
	size_t num_memb;
	void** members;
} TypeInfo;

typedef struct VECTOR {
	TypeIndex type;
	size_t len;
	size_t cap;
	uint8_t data[];
} Vector;

// TODO: the mark and label fields could be combined into a single
// "flag" field, but would this actually save space in the struct or
// would padding be added anyway?
typedef struct METADATA {
	bool mark;
	enum { DATA, INDEX
	} label;
	union {
		TypeIndex type;	// DATA
		size_t index;	// INDEX
	};
} Metadata;


TypeIndex type_init(size_t struct_sz, size_t num_memb, void** members);
TypeInfo* type_get_info(TypeIndex t);
size_t type_get_size(TypeIndex t);
bool type_equal(TypeIndex t1, TypeIndex t2);
void type_free();

Metadata* metadata_get(void* data);


#define type_memb(type, mem) (void*)(&((type*)0)->mem)

#define type_setup(NAME)								\
	TypeIndex TYPE_##NAME(){							\
		static bool is_initialized = false;				\
		static TypeIndex t;								\
		if(!is_initialized){ 							\
			is_initialized = true;						\
			t = INIT_##NAME();							\
		}												\
		return t;										\
	}


#define primitive_setup(NAME, type) 					\
	TypeIndex INIT_##NAME(){							\
		return type_init(sizeof(type), 0, NULL);		\
	}													\
	type_setup(NAME)


#define struct_setup(NAME, type, ...) 					\
	TypeIndex INIT_##NAME(){							\
		void* members[] = { __VA_ARGS__ };				\
		return type_init(								\
			sizeof(type), 								\
			sizeof(members)/sizeof(*members), 			\
			members										\
		);												\
	}													\
	type_setup(NAME)

TypeIndex TYPE_POINTER();
TypeIndex TYPE_VECTOR();
TypeIndex TYPE_BOOL();
TypeIndex TYPE_CHAR();
TypeIndex TYPE_INT();
TypeIndex TYPE_FLOAT();
TypeIndex TYPE_DOUBLE();
TypeIndex TYPE_LONG_DOUBLE();
TypeIndex TYPE_INT8();
TypeIndex TYPE_INT16();
TypeIndex TYPE_INT32();
TypeIndex TYPE_INT64();

#endif
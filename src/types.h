#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct TYPE_INDEX {
	size_t index;
} TypeIndex;

typedef struct TYPE_INFO {
	size_t struct_sz;
	size_t num_memb;
	void** members;
} TypeInfo;

#define TYPE_INIT_SIZE 4
#define TYPE_INIT_RATIO 2

TypeIndex type_init(size_t struct_sz, size_t num_memb, void** members);
TypeInfo* type_get_info(TypeIndex t);
void type_free();

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

TypeIndex INIT_POINTER();
TypeIndex TYPE_POINTER();
TypeIndex INIT_BOOL();
TypeIndex TYPE_BOOL();
TypeIndex INIT_CHAR();
TypeIndex TYPE_CHAR();
TypeIndex INIT_INT();
TypeIndex TYPE_INT();
TypeIndex INIT_FLOAT();
TypeIndex TYPE_FLOAT();
TypeIndex INIT_DOUBLE();
TypeIndex TYPE_DOUBLE();
TypeIndex INIT_LONG_DOUBLE();
TypeIndex TYPE_LONG_DOUBLE();
TypeIndex INIT_INT8();
TypeIndex TYPE_INT8();
TypeIndex INIT_INT16();
TypeIndex TYPE_INT16();
TypeIndex INIT_INT32();
TypeIndex TYPE_INT32();
TypeIndex INIT_INT64();
TypeIndex TYPE_INT64();


#endif
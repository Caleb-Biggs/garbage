#ifndef GARBAGE_H
#define GARBAGE_H

#include <stdbool.h>
#include <stdint.h>


typedef struct TYPE_INDEX {
	size_t index;
} TypeIndex;

typedef struct VECTOR {
	TypeIndex type;
	size_t len;
	size_t cap;
	uint8_t data[];
} Vector;


int start_garbage_collector();
void end_garbage_collector();
void start_function();
void end_function(void* ret);
void* gc_alloc(TypeIndex t);
// void* gc_alloc_array(TypeIndex t, size_t num);
// void gc_resize_array(Array* a, size_t num);

// Debug
void graph_print_memory();

// Type Stuff
TypeIndex type_init(size_t struct_sz, size_t num_memb, void** members);

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
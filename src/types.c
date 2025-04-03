#include <string.h>
#include "types.h"


static size_t next_index = 0;
static size_t curr_size = TYPE_INIT_SIZE;
static TypeInfo* types = NULL;


TypeIndex type_init(size_t struct_sz, size_t num_memb, void** members){
	if(next_index >= curr_size || types == NULL){
		curr_size *= TYPE_INIT_RATIO;
		types = realloc(types, sizeof(*types)*curr_size);
	}

	size_t bytes = sizeof(*members) * num_memb;
	types[next_index] = (TypeInfo){struct_sz, num_memb, malloc(bytes)};
	memcpy(types[next_index].members, members, bytes);

	return (TypeIndex){next_index++};
}


TypeInfo* type_get_info(TypeIndex t){
	if(t.index >= next_index || !types) return NULL;
	return types+t.index;
}
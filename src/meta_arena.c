#include <stdio.h>
#include "meta_arena.h"


int meta_arena_new(size_t data_size, MetaArena* output){
	if(!output) return -1;
	size_t max_arenas = STARTING_ARENAS;
	size_t* arena_next = malloc(max_arenas * sizeof(*arena_next));
	Arena* arenas = malloc(max_arenas * sizeof(*arenas));
	if(!arenas || !arena_next) return -2;
	for(size_t i = 0; i < max_arenas; i++) arena_next[i] = i+1;
	
	*output = (MetaArena){
		.fill_next = 0,
		.num_arenas = 0,
		.max_arenas = max_arenas,
		.data_size = data_size,
		.arena_next = arena_next,
		.arenas = arenas
	};
	return 0;
}


void meta_arena_free(MetaArena* m){
	if(!m) return;
	for(size_t i = 0; i < m->num_arenas; i++) 
		arena_free(m->arenas+i);
	free(m->arenas);
	free(m->arena_next);
	m->arenas = NULL;
	m->arena_next = NULL;
}


int meta_arena_push(MetaArena* m){
	if(m->fill_next >= m->max_arenas){
		size_t temp = m->max_arenas;
		m->max_arenas = 1 + m->max_arenas * META_ARENA_RESIZE;
		if(!(m->arenas = realloc(m->arenas, sizeof(*(m->arenas))*m->max_arenas)) ||
			!(m->arena_next = realloc(m->arena_next, sizeof(*(m->arena_next))*m->max_arenas))
		) return -2;
		for(size_t i = temp; i < m->max_arenas; i++) 
			m->arena_next[i] = i+1;
	}
	return arena_new(m->data_size, m->arenas+(m->num_arenas++));
}


void* meta_arena_allocate(MetaArena* m, TypeIndex t){
	if(!m) return NULL;
	if(m->fill_next >= m->num_arenas)
		meta_arena_push(m); //TODO: Error check

	size_t l; size_t* loc = &l;
	if(!arena_insert(m->arenas+m->fill_next, &loc, t)) 
		return NULL;

	void* output = arena_get_data(m->arenas+m->fill_next, l);
	if(arena_is_full(m->arenas[m->fill_next])) 
		m->fill_next = m->arena_next[m->fill_next];	

	return output;
}


void meta_arena_delete_unmarked(MetaArena* m){
	// Delete all marked data
	for(size_t i = 0; i < m->num_arenas; i++)
		arena_delete_unmarked(m->arenas+i);

	// Remove empty arenas
	size_t remove = 0;
	for(size_t i = 0; i < m->num_arenas; i++){
		if(arena_is_empty(m->arenas[i])){
			remove++;
			arena_free(m->arenas+i);
		} else if(remove != 0){
			m->arenas[i-remove] = m->arenas[i];
		}
	}
	m->num_arenas -= remove;

	// Rebuild arena_next list
	size_t prev = m->num_arenas;
	for(ssize_t i = m->num_arenas-1; i >= 0; i--){
		if(arena_is_full(m->arenas[i])) continue;
		m->arena_next[i] = prev;
		prev = i;
	}
	m->fill_next = prev;
}


// Debug method, not error checked
void meta_arena_mark_delete(MetaArena* m, size_t arena, size_t index){
	arena_mark_for_deletion(m->arenas+arena, index);
}


void meta_arena_print(MetaArena m){
	printf("Fill next: %lu\n", m.fill_next);
	printf("Number of Arenas: %lu\n", m.num_arenas);
	printf("Data size: %lu\n\n", m.data_size);
	for(size_t i = 0; i < m.num_arenas; i++){
		printf("Arena %lu\n", i);
		arena_print(m.arenas+i);
		printf("\n");
	}
}
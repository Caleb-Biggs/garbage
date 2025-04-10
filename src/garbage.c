#include <stdio.h>
#include "garbage.h"
#include "metadata.h"
#include "arena_manager.h"


typedef struct FUNCTION {
	size_t num_children;
	Array* children;
} Function;
struct_setup(FUNCTION, Function, 
	type_memb(Function, children)
)

void function_push(Function* f, void* child);
void* function_pop(Function* f);

typedef struct MEMORY_LIST {
	Function* func;
	struct MEMORY_LIST* next;
} List;

void list_push(List** l, Function* func);
List* list_pop(List** l);
void list_free(List** l);


static ArenaManager memory = {0};
static Function* root = NULL;
static List* context = NULL;


int start_garbage_collector(){
	int ret = manager_new(&memory);
	if(ret != 0) return ret;
	start_function();
	root = context->func;
	return 0;
}


void end_garbage_collector(){
	manager_free(&memory);
	type_free();
	list_free(&context);
}


void start_function(){
	Array* children = array_new(TYPE_POINTER(), 3);
	Function* func = gc_alloc(TYPE_FUNCTION());
	func->children = children;
	list_push(&context, func);
}


void end_function(void* ret){
	List* popped = list_pop(&context);
	Function* func_a = popped->func;
	Function* func_b = function_pop(context->func);
	function_pop(context->func);
	function_pop(context->func);
	if(func_a != func_b) printf("AHHHHHH\n");
	free(popped);	
	if(ret == NULL) return;
	function_push(context->func, ret);
}


void* gc_alloc(TypeIndex t){
	void* output = manager_allocate(&memory, t);
	if(root != NULL) function_push(context->func, output);
	return output;
}


void* gc_alloc_array(TypeIndex t, size_t num){
	void** arr = gc_alloc(TYPE_POINTER());
	if(!arr) return NULL;
	*arr = calloc(num, type_get_info(t)->struct_sz);
	// printf("Manual alloc: %p\n", *arr);
	return arr;
}

// Garbage Collector


void graph_traversal(void* node){
	Metadata* m = metadata_get(node);
	if(m->mark) return;
	m->mark = true;
	TypeInfo t = *type_get_info(m->type);
	
	for(size_t i = 0; i < t.num_memb; i++){
		graph_traversal(*(void**)((uint8_t*)node + (uint64_t)t.members[i]));
	}
	
	if(m->type.index == TYPE_ARRAY().index){
		Array* a = node;
		if(a->type.index == TYPE_POINTER().index){
			for(size_t i = 0; i < a->len; i++){
				if(*(*(void***)a->data + i) == NULL) break;
				graph_traversal(*(*(void***)a->data + i));
			}
		}
	}
}


void run_garbage_collection(){
	graph_traversal(root);
	manager_delete_unmarked(memory);
}

// Debug Functions

void graph_print_memory(){
	manager_print(memory);
}

//

void function_push(Function* f, void* child){
	if(f->num_children >= f->children->len) array_resize(f->children, 2*f->num_children);
	*((void**)(*(void**)f->children->data)+f->num_children) = child;
	f->num_children++;
}


void* function_pop(Function* f){
	f->num_children--;
	void* output = *((void**)(*(void**)f->children->data)+f->num_children);
	*((void**)(*(void**)f->children->data)+f->num_children) = NULL;
	return output;
}


// Assumes l is not NULL, *l can be NULL
void list_push(List** l, Function* func){
	List* new = malloc(sizeof(*new));
	new->func = func;
	new->next = *l;
	*l = new;
}


List* list_pop(List** l){
	if(!(*l)) return NULL;
	List* output = *l;
	*l = output->next;
	return output;
}


void list_free(List** l){
	List* node;
	while((node = list_pop(l)) != NULL) free(node);
}
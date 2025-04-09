#include <stdio.h>
#include "garbage.h"


typedef struct FUNCTION {
	size_t num_children;
	Array children;
} Function;
struct_setup(FUNCTION, Function, 
	type_memb(Function, children.data)
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
	// printf("Start %s\n", __func__);
	Array children = array_new_pnt(2);
	Function* func = gc_alloc(TYPE_FUNCTION());
	func->children = children;
	list_push(&context, func);
	// printf("End %s\n", __func__);
}


void end_function(void* ret){
	// printf("Start %s\n", __func__);
	List* popped = list_pop(&context);
	Function* func_a = popped->func;
	// printf("Context Popped: %p\n", (void*)func_a);
	Function* func_b = function_pop(context->func);
	// printf("Function Popped: %p\n", (void*)func_b);
	if(func_a != func_b) printf("AHHHHHH\n");
	free(popped);
	
	if(ret == NULL) {
		// printf("End %s\n", __func__);
		return;
	}
	function_push(context->func, ret);
	// printf("End %s\n", __func__);
}


void* gc_alloc(TypeIndex t){
	// printf("Start %s\n", __func__);
	void* output = manager_allocate(&memory, t);
	if(root != NULL) function_push(context->func, output);
	// printf("End %s\n", __func__);
	return output;
}


void** gc_alloc_array(TypeIndex t, size_t num, bool is_pointer){
	// printf("Start %s\n", __func__);
	void** arr = gc_alloc((is_pointer) ? TYPE_POINTER_ARR() : TYPE_POINTER());
	// printf("ARRAY: %p; is_pointer: %s\n", (void*)arr, (is_pointer) ? "true" : "false");
	if(!arr) return NULL;
	*arr = calloc(num, type_get_info(t)->struct_sz);
	// printf("gc_alloc_array: %p\n", *arr);
	// printf("End %s\n", __func__);
	return arr;
}

// Garbage Collector

void graph_traversal(void* node){
	Metadata* m = metadata_get(node);
	if(m->mark) return;
	printf("Checking: %p\n", node);
	m->mark = true;
	TypeInfo t = *type_get_info(m->type);
	// printf("Pointer: %p; TYPE: %lu\n", node, m->index);
	if(m->type.index == TYPE_FUNCTION().index){
		printf("POINTER ARR\n");
		Function* func = node;
		printf("num_children: %lu\n", func->num_children);
		for(size_t i = 0; i < func->num_children; i++){
			if(func->children.data[i] == NULL) break;
			printf("POINTER: %p\n", *(void**)(func->children.data[i]));
			// graph_traversal(*(void**)func->children.data[i]);
		}
	}
	for(size_t i = 0; i < t.num_memb; i++){
		graph_traversal(*(void**)((uint8_t*)node + (uint64_t)t.members[i]));
	}

}


void run_garbage_collection(){
	graph_traversal(root);
	// manager_delete_marked(memory);
}

// Debug Functions

void graph_print_memory(){
	manager_print(memory);
}

//

void function_push(Function* f, void* child){
	if(f->num_children >= f->children.len)
		array_resize(&f->children, 2*(f->num_children));
	// printf("Push: %p\n", (void*)((void**)(*f->children.data)+f->num_children));
	printf("Pushing %p\n", child);
	*(void**)array_get(f->children, f->num_children) = child;
	// *((void**)(*f->children.data)+f->num_children) = child;
	// ((void**)f->children.data)[f->num_children] = child;
	f->num_children++;
}


void* function_pop(Function* f){
	f->num_children--;
	// void* output = ((void**)f->children.data)[f->num_children];
	void* output = *(void**)array_get(f->children, f->num_children);
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
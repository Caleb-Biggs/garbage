#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
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
static pthread_mutex_t mutex;
static pthread_t collector;
static bool running = false;


int start_garbage_collector(){
	pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&mutex, &attr);
	pthread_mutex_lock(&mutex);
	pthread_create(&collector, NULL, run_garbage_collection, NULL);

	running = true;
	int ret = manager_new(&memory);
	if(ret != 0) return ret;
	start_function();
	root = context->func;
	pthread_mutex_unlock(&mutex);
	return 0;
}


void end_garbage_collector(){
	pthread_mutex_lock(&mutex);
	running = false;
	pthread_mutex_unlock(&mutex);
	pthread_join(collector, NULL);

	manager_free(&memory);
	type_free();
	list_free(&context);

}


void start_function(){
	pthread_mutex_lock(&mutex);
	Array* children = array_new(TYPE_POINTER(), 3);
	Function* func = gc_alloc(TYPE_FUNCTION());
	func->children = children;
	list_push(&context, func);
	pthread_mutex_unlock(&mutex);
}


void end_function(void* ret){
	pthread_mutex_lock(&mutex);
	List* popped = list_pop(&context);
	Function* func_a = popped->func;
	Function* func_b = function_pop(context->func);
	function_pop(context->func);
	function_pop(context->func);
	if(func_a != func_b) printf("AHHHHHH\n");
	free(popped);	
	if(ret != NULL) function_push(context->func, ret);
	pthread_mutex_unlock(&mutex);
}


void* gc_alloc(TypeIndex t){
	pthread_mutex_lock(&mutex);
	void* output = manager_allocate(&memory, t);
	if(root != NULL) function_push(context->func, output);
	pthread_mutex_unlock(&mutex);
	return output;
}


void* gc_alloc_array(TypeIndex t, size_t num){
	pthread_mutex_lock(&mutex);
	void** arr = gc_alloc(TYPE_POINTER());
	if(!arr) return NULL;
	*arr = calloc(num, type_get_info(t)->struct_sz);
	pthread_mutex_unlock(&mutex);
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


void* run_garbage_collection(void* _){
	while(running){
		usleep(100000); // 100ms
		pthread_mutex_lock(&mutex);
		graph_traversal(root);
		manager_delete_unmarked(memory);
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
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
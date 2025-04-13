#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "garbage.h"
#include "metadata.h"
#include "arena_manager.h"


typedef struct VECTOR {
	TypeIndex type;
	size_t len;
	size_t cap;
	uint8_t data[];
} Vector;
primitive_setup(VECTOR, Vector)

Vector** function_new();
bool function_push(Vector** v, void* item);
void* function_pop(Vector* v);
Vector** function_prev(Vector* v);


static ArenaManager memory = {0};
static Vector** root = NULL;
static Vector** context = NULL;
static pthread_mutex_t* mutex = NULL;
static pthread_t collector;
static bool running = false;


// Interface


int start_garbage_collector(){
	// Global Setup
	int ret = manager_new(&memory);
	if(ret != 0) return ret;
	start_function();
	root = context;
	running = true;

	// Thread Setup
	static pthread_mutex_t mutex_local;
	mutex = &mutex_local;
	pthread_mutex_init(mutex, NULL);
	pthread_mutex_lock(mutex);
	int thr = pthread_create(&collector, NULL, run_garbage_collection, NULL);
	if(thr != 0) printf("pthread_create result: %i\n", thr);
	pthread_mutex_unlock(mutex);
	return 0;
}


void end_garbage_collector(){
	// graph_print_memory();
	pthread_mutex_lock(mutex);
	running = false;
	pthread_mutex_unlock(mutex);
	pthread_join(collector, NULL);
	manager_free(&memory);
	type_free();
}


void start_function(){
	if(mutex) pthread_mutex_lock(mutex);

	// printf("start_function: START\n");
	Vector** func = function_new();
	function_push(func, context);
	if(context) function_push(context, func);
	// else printf("start_function: NULL context\n");
	context = func;
	// printf("start_function: END\n");

	if(mutex) pthread_mutex_unlock(mutex);
}


void end_function(void* ret){
	pthread_mutex_lock(mutex);

	// printf("%s: START\n", __func__);
	Vector** ending = context;
	if(context) context = function_prev(*context);
	// else printf("end_function: NULL Context\n"); 
	
	Vector** popped = NULL;
	if(context) popped = function_pop(*context);
	// else printf("end_function: NULL Context\n");
	
	// printf("Popped: %p\n", (void*)popped);
	if(!popped);// printf("end_function: NULL\n");
	else if(popped != ending) {
		printf("AHHHHH\n");
		printf("Ending: %p; Popped: %p; Next: %p\n", (void*)ending, (void*)popped, (void*)context);
	}
	// else printf("end_function: VALID\n");
	if(ret) function_push(context, ret);
	// printf("%s: END\n", __func__);

	pthread_mutex_unlock(mutex);
}


void* gc_alloc(TypeIndex t){
	pthread_mutex_lock(mutex);
	// printf("gc_alloc: START\n");
	void* output = manager_allocate(&memory, t);
	function_push(context, output);
	// printf("gc_alloc: END\n");
	pthread_mutex_unlock(mutex);
	return output;
}


// Garbage Collector


void graph_traversal(void* node){
	if(!node) return;
	Metadata* m = metadata_get(node);
	if(m->mark) return;
	// printf("Visiting %p\n", node);
	m->mark = true;
	TypeInfo t = *type_get_info(m->type);
	
	for(size_t i = 0; i < t.num_memb; i++){
		graph_traversal(*(void**)((uint8_t*)node + (size_t)t.members[i]));
	}

	if(m->type.index == TYPE_VECTOR().index){
		Vector* v = node;
		if(v->type.index == TYPE_POINTER().index){
			for(size_t i = 0; i < v->len; i++){
				size_t offset = i * type_get_info(TYPE_POINTER())->struct_sz;
				graph_traversal(*(void**)(v->data + offset));
			}
		}
	}
}


void* run_garbage_collection(void* _){
	int ran = 0;
	while(running){
		usleep(100000); // 100ms
		pthread_mutex_lock(mutex);
		// printf("STARTING GARBAGE COLLECTION\n");
		graph_traversal(root);
		manager_delete_unmarked(memory);
		ran++;
		// printf("ENDING GARBAGE COLLECTION\n");
		pthread_mutex_unlock(mutex);
		// break;
	}
	printf("GC RAN %i TIMES\n", ran);
	return NULL;
}


// Function functions


Vector* function_new_helper(size_t size){
	// Vector** output = manager_allocate(&memory, TYPE_POINTER());
	Vector* output = manager_allocate_arbitrary(&memory, TYPE_VECTOR(),
		sizeof(Vector) + (size * type_get_info(TYPE_POINTER())->struct_sz));
	output->type = TYPE_POINTER();
	output->cap = size;
	return output;
}

Vector** function_new(){
	Vector** output = manager_allocate(&memory, TYPE_POINTER());
	*output = function_new_helper(2);
	return output;
}

bool function_push(Vector** v, void* item){
	if(!v || !(*v)) return false;
	// printf("Assigning %p to %p at pos %lu\n", item, (void*)*v, (**v).len);
	size_t data_size = type_get_info((**v).type)->struct_sz;
	if((**v).len == (**v).cap){
		Vector* new = function_new_helper(2 * (**v).cap);
		if(!new) return false;
		new->len = (**v).len;
		memcpy(new->data, (**v).data, (**v).len*data_size);
		// printf("R Resizing for %p\n", item);
		*v = new;
	}
	memcpy((**v).data + ((**v).len*data_size), &item, data_size);
	(**v).len++;
	return true;
}

void* function_pop(Vector* v){
	if(!v || v->len == 0){
		// printf("Returning NULL\n");
		return NULL;
	}
	v->len--;
	// printf("Popping pos %lu from %p\n", v->len, (void*)v);
	return *(void**)(v->data + (v->len * type_get_info(v->type)->struct_sz));
}

Vector** function_prev(Vector* v){
	if(!v){
		// printf("function_prev: returning NULL\n");
		return NULL;
	}
	Vector** output = *(Vector***)v->data;
	// printf("funtion_prev: returning %p\n", (void*)output);
	return output;
}


// Debug Functions


void graph_print_memory(){
	pthread_mutex_lock(mutex);
	manager_print(memory);
	pthread_mutex_unlock(mutex);
}
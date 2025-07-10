#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "garbage.h"
#include "arena_manager.h"
#include "types.h"
#include "../vis/visualization.h"


Vector** function_new();
bool function_push(Vector** v, void* item);
void* function_pop(Vector* v);
Vector** function_prev(Vector* v);
void* run_garbage_collection(void* _);


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
	pthread_mutex_lock(mutex);
	running = false;
	pthread_mutex_unlock(mutex);
	pthread_join(collector, NULL);
	manager_free(&memory);
	type_free();
}


void start_function(){
	if(mutex) pthread_mutex_lock(mutex);

	Vector** func = function_new();
	function_push(func, context);
	if(context) function_push(context, func);
	context = func;

	VIS_GC_START_FUNCTION(*context);
	if(mutex) pthread_mutex_unlock(mutex);
}


void end_function(void* ret){
	pthread_mutex_lock(mutex);
	VIS_GC_END_FUNCTION(*context);

	Vector** ending = context;
	if(context) context = function_prev(*context);
	
	Vector** popped = NULL;
	if(context) popped = function_pop(*context);
	
	if(!popped);
	else if(popped != ending) {
		printf("AHHHHH\n");
		printf("Ending: %p; Popped: %p; Next: %p\n", (void*)ending, (void*)popped, (void*)context);
	}
	if(ret) function_push(context, ret);

	pthread_mutex_unlock(mutex);
}


void* gc_alloc(TypeIndex t){
	pthread_mutex_lock(mutex);

	void* output = manager_allocate(&memory, t);
	function_push(context, output);

	VIS_GC_ALLOC(output);
	pthread_mutex_unlock(mutex);
	return output;
}


// Garbage Collector


void graph_traversal(void* node){
	if(!node) return;
	Metadata* m = metadata_get(node);
	if(m->mark) return;
	VIS_GC_GRAPH_TRAVERSAL(node);
	m->mark = true;
	TypeInfo t = *type_get_info(m->type);
	
	for(size_t i = 0; i < t.num_memb; i++){
		graph_traversal(*(void**)((uint8_t*)node + (size_t)t.members[i]));
	}

	if(type_equal(m->type, TYPE_VECTOR()) && 
	type_equal(((Vector*)node)->type, TYPE_POINTER())){
		Vector* v = node;
		for(size_t i = 0; i < v->len; i++){
			size_t offset = i * type_get_size(TYPE_POINTER());
			graph_traversal(*(void**)(v->data + offset));
		}
	}
}


void* run_garbage_collection(void* _){
	(void)_;
	int ran = 0;
	while(running){
		usleep(100000); // 100ms
		pthread_mutex_lock(mutex);
		VIS_GC_COLLECT();
		graph_traversal(root);
		manager_delete_unmarked(memory);
		ran++;
		pthread_mutex_unlock(mutex);
	}
	// printf("GC RAN %i TIMES\n", ran);
	return NULL;
}


// Function functions


Vector* function_new_helper(size_t size){
	Vector* output = manager_allocate_arbitrary(&memory, TYPE_VECTOR(),
		sizeof(Vector) + (size * type_get_size(TYPE_POINTER())));
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
	size_t data_size = type_get_size((**v).type);
	if((**v).len == (**v).cap){
		Vector* new = function_new_helper(2 * (**v).cap);
		if(!new) return false;
		new->len = (**v).len;
		memcpy(new->data, (**v).data, (**v).len*data_size);
		*v = new;
	}
	memcpy((**v).data + ((**v).len*data_size), &item, data_size);
	(**v).len++;
	return true;
}

void* function_pop(Vector* v){
	if(!v || v->len == 0) return NULL;
	v->len--;
	return *(void**)(v->data + (v->len * type_get_size(v->type)));
}

Vector** function_prev(Vector* v){
	if(!v) return NULL;
	Vector** output = *(Vector***)v->data;
	return output;
}


// Debug Functions


void graph_print_memory(){
	pthread_mutex_lock(mutex);
	manager_print(memory);
	pthread_mutex_unlock(mutex);
}
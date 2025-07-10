#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#ifdef ENABLE_VIS

#define FILE_GARBAGE 'G'
#define FUNC_START_FUNCTION	'S'
#define FUNC_END_FUNCTION 'E'
#define FUNC_ALLOC 'A'
#define FUNC_COLLECT 'C'
#define FUNC_TRAVERSAL 'T'

static inline void VIS_TIME(char* output){
	struct timespec t;
	clock_gettime(CLOCK_REALTIME, &t);
	snprintf(output, 42, "%20lu %20lu", t.tv_sec, t.tv_nsec);
}

static inline void VIS_PRINT(char file, char func, void* ptr){
	char time[42];
	VIS_TIME(time);
	printf("%c %c %s %20p\n", file, func, time, ptr);
}

static inline void VIS_GC_START_FUNCTION(void* ptr){
	VIS_PRINT(FILE_GARBAGE, FUNC_START_FUNCTION, ptr);
}


static inline void VIS_GC_END_FUNCTION(void* ptr){
	VIS_PRINT(FILE_GARBAGE, FUNC_END_FUNCTION, ptr);
}


static inline void VIS_GC_ALLOC(void* ptr){
	VIS_PRINT(FILE_GARBAGE, FUNC_ALLOC, ptr);
}


static inline void VIS_GC_COLLECT(){
	VIS_PRINT(FILE_GARBAGE, FUNC_COLLECT, NULL);
}


static inline void VIS_GC_GRAPH_TRAVERSAL(void* ptr){
	VIS_PRINT(FILE_GARBAGE, FUNC_TRAVERSAL, ptr);
}

#else

static inline void TEST_PRINT(){}
static inline void VIS_GC_START_FUNCTION(){}
static inline void VIS_GC_END_FUNCTION(){}
static inline void VIS_GC_ALLOC(){}
static inline void VIS_GC_COLLECT(){}
static inline void VIS_GC_GRAPH_TRAVERSAL(){}

#endif
#endif
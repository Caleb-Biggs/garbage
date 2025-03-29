## About
This is a garbage collector, written in C.  
This program currently only supports being run in a single-threaded application.
The main goal of this program is to eventually be used in a programming language I plan to create at some point, so usability by a human programmer is not my top priority, instead it'll be inserted into a program at compile time.

## Usage
Header file: ```#include "garbage.h"```  
Intialize the garbage collector at the beginning of the program and stop it at the end:  
```C
int main(int argc, char const* argv[]){
	start_garbage_collection();
	// Code
	end_garbage_collection();
}
```
Indicate to the garbage collector that context has or will change when entering and leaving a function, allowing it to connect newly allocated data to the correct node in the memory graph:
```C
void my_func(){
	start_function();
	// Code
	end_function(NULL);
}
```
Allocate memory, returning a handle used to access the corresponding pointer:
```C
MemLoc arr = gc_alloc(sizeof(int)*10);
for(int i = 0; i < 10; i++){
	((int*)get(arr))[i] = i*i;
}
```
When returning a variable allocated with ```gc-alloc()```, include it as an argument in ```end_function``` so it doesn't get removed from the memory graph at the end of the function:
```C
MemLoc get_squares(int x){
	start_function();

	MemLoc arr = gc_alloc(sizeof(int)*x);
	for(int i = 0; i < x; i++){
		((int*)get(arr))[i] = i*i;
	}

	return end_function(&arr);
}
```
Connections in the memory graph can be manually added or removed. This should only need to be done when initializing a struct that uses needs more than one heap allocation, or when changing a struct's pointer to newly allocated space:
```C
typedef struct MY_STRUCT {
	int* p1;
	char* p2;
} MyStruct;

MemLoc my_struct_new(int size){
	start_function();
	MemLoc output = gc_alloc(sizeof(MyStruct));
	MemLoc p1 = gc_alloc(size * sizeof(int));
	MemLoc p2 = gc_alloc(size * sizeof(char));
	((MyStruct*)get(output))->p1 = get(p1);
	((MyStruct*)get(output))->p2 = get(p2);
	attach(output, p1);
	attach(output, p2);
	return end_function(output);
}

void my_struct_resize(MemLoc ms, int new_size){
	// TODO: detach
}
```

## Future
- Get garbage collection working (Complete)
- Improve error handling (In-progress)
- Write tests (In-progress)
- Put garbage collector on its own thread (Not started)
- Make allocation thread-safe (Not started)
- Simplify interface (Not started)
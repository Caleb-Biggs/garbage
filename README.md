## About
This is a garbage collector, written in C.  
This program currently only supports being run in a single-threaded application.  
The main goal of this program is to eventually be used in a programming language I plan to create at some point, so usability by a human programmer is not my top priority, instead it'll be inserted into a program at compile time.

## Usage
Header file: ```#include "garbage.h"```  
Intialize the garbage collector at the beginning of the program and stop it at the end:  
```C
int main(int argc, char const* argv[]){
	start_garbage_collector();
	// Code
	end_garbage_collector();
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
Allocate memory for a single data type, or an array, similar to calloc:
```C
char* c = gc_alloc(TYPE_CHAR()); 			 // Allocates a single char
char* str = gc_alloc_array(TYPE_CHAR(), 10); // Allocates 10 chars
```
When returning a variable allocated with ```gc_alloc()```, include it as an argument in ```end_function``` so it doesn't get removed from the memory graph at the end of the function:
```C
char* first_n_letters(int n){
	start_function();

	char* str = gc_alloc_array(TYPE_CHAR(), n);
	for(int i = 0; i < n; i++){
		str[i] = 'A' + i;
	}

	end_function(str);
	return str;
}
```
The garbage collector recognizes a number of primitive types, all with the `TYPE_` prefix, along with the Array type (`TYPE_ARRAY`). For a full list of types included, see `src/types.h`. To declare a new type, there are a few macros provided:
```C
typedef enum ERROR { 
	NULL_PTR,
	INDEX_OOB,
	OUT_OF_MEM
} Error;

// This data type contains no pointers, so it can be declared as "primitive"
// 'ERROR' can be any name you'd like, 'Error' is the type's actual name
// This macro and the next generate a function 'TYPE_ARG1()' that can be
// passed as an argument to gc_alloc()
primitive_setup(ERROR, Error)	


typedef struct BINARY_TREE {
	struct TREE* left;
	struct TREE* right;
} Tree;

// This data type contains pointers that will need to be tracked by
// the garbage collector, so the following can be used
struct_setup(TREE, Tree,		
	type_memb(Tree, left),	// The type is the first argument
	type_memb(Tree, right)	// The name of the member is the second
)

int main(int argc, char* argv[]){
	start_garbage_collector();
	Error* e = gc_alloc(TYPE_ERROR());
	Tree* root = gc_alloc(TYPE_TREE());
	root->left = gc_alloc(TYPE_TREE());
	root->right = gc_alloc(TYPE_TREE());
```



## Future
- Get garbage collection working (Complete)
- Simplify interface (Complete)
- Put garbage collector on its own thread (Probably complete)
- Improve error handling (In-progress)
- Write tests (In-progress)
- Make allocation thread-safe (Not started)
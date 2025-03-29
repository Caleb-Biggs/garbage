#include "garbage.h"


int main(int argc, char const* argv[]){
	start_garbage_collection();


		start_function();
		// MemLoc l1 = gc_alloc(sizeof(char));
		// *(char*)get(l1) = 'C';

		// MemLoc l2 = gc_alloc(sizeof(char));
		// *(char*)get(l2) = 'A';

		char* str = "abcdefghijklmnopqrstuvwxyz";
		for(int i = 0; i < 100; i++){
			MemLoc l = gc_alloc(sizeof(char));
			*(char*)get(l) = str[i%26];
		}
		print_graph();
		collect_garbage();
		print_graph();

		// attach(l1, l2);
		// print_graph();
		// detach(l1, l2);

		end_function(NULL);
	
	collect_garbage();
	print_graph();
	// MemLoc l1 = gc_alloc(sizeof(char));
	// *(char*)get(l1) = 'C';
	// print_graph();


	end_garbage_collection();
}
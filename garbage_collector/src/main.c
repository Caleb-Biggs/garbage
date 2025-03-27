#include "garbage.h"


int main(int argc, char const* argv[]){
	start_garbage_collection();


		start_function();
		MemLoc l1 = gc_alloc(sizeof(char));
		*(char*)get(l1) = 'C';

		MemLoc l2 = gc_alloc(sizeof(char));
		*(char*)get(l2) = 'A';


		attach(l1, l2);
		print_graph();
		detach(l1, l2);

		end_function(&l2);
	
	// collect_garbage();
	print_graph();


	end_garbage_collection();
}
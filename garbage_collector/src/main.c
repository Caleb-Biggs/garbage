#include "garbage.h"


int main(int argc, char const* argv[]){
	start_garbage_collection();


		start_function();
		MemLoc l1 = gc_alloc(sizeof(char));
		*(char*)get(l1) = 'C';

		MemLoc l2 = gc_alloc(sizeof(char));
		*(char*)get(l2) = 'A';

		attach(l1, l2);
		detach(l1, l2);

		end_function();


	end_garbage_collection();
}
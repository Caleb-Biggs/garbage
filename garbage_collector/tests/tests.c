#include <check.h>
#include <stdio.h>  
#include <stdlib.h>
#include <stdio.h>
#include "bitfield_tests.c"


#define MAX_SUITES 1
int parse_args(Suite** suites, int argc, char const* argv[]){
	if(argc == 1) return -1;
	if(argc > MAX_SUITES+1) return -2;

	int num_valid_args = 0;
	for(int i = 1; i < argc; i++){
		if(argv[i][0] != '-') continue;
		switch(argv[i][1]){
			case 'b': suites[num_valid_args++] = bitfield_suite(); break;
			// case 'h': suites[num_valid_args++] = (); break;
			// case 'n': suites[num_valid_args++] = (); break;
			// case 'a': suites[num_valid_args++] = (); break;
			// case 'g': suites[num_valid_args++] = (); break;
		}
	}
	return num_valid_args;
}


int main(int argc, char const* argv[]){
	Suite* suites[MAX_SUITES] = {0};
	int num_suites = parse_args(suites, argc, argv);
	if(num_suites == -1){
		suites[0] = bitfield_suite();
		// suites[1] = ();
		// suites[2] = ();
		// suites[3] = ();
		// suites[4] = ();
		num_suites = MAX_SUITES;
	}

	int failed = 0;
	for(int i = 0; i < num_suites; i++){
		SRunner* runner = srunner_create(suites[i]);
		srunner_run_all(runner, CK_NORMAL);
		failed += srunner_ntests_failed(runner);
		srunner_free(runner);
	}

	return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
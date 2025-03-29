#include <check.h>
#include <stdio.h>
#include "../src/hashset.h"

START_TEST(hash_set_funcs_reject_null){
	printf("\tFinished %s\n", __func__);
} END_TEST


Suite* hashset_suite(void){
	Suite* suite;
	TCase* core;

	suite = suite_create("HashSet Suite");
	core = tcase_create("Core");
	tcase_add_test(core, hash_set_funcs_reject_null);
	suite_add_tcase(suite, core);

	return suite;
}
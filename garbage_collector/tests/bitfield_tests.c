#include <check.h>

START_TEST(bit_field_new_test){
	printf("Hello world\n");
}END_TEST


Suite* bitfield_suite(void){
	Suite* suite;
	TCase* core;

	suite = suite_create("Bitfield Suite");
	core = tcase_create("Core");
	tcase_add_test(core, bit_field_new_test);
	suite_add_tcase(suite, core);

	return suite;
}
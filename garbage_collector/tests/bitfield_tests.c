#include <check.h>
#include <stdio.h>
#include "../src/bitfield.h"

START_TEST(bit_field_funcs_reject_null){
	BitField invalid_bits = {NULL};
	ck_assert_int_eq(bit_field_get(invalid_bits, 0), 	-1);
	ck_assert_int_eq(bit_field_set(NULL, 0, 0), 		-1);
	ck_assert_int_eq(bit_field_set(&invalid_bits, 0, 0),-1);
	ck_assert_int_eq(bit_field_clear(NULL, 0), 			-1);
	ck_assert_int_eq(bit_field_clear(&invalid_bits, 0), -1);
	printf("\tFinished %s\n", __func__);
} END_TEST


START_TEST(bit_field_set_and_get){
	const int SIZE = 100;
	BitField* b = bit_field_new(SIZE);
	for(int i = 0; i < SIZE; i++){
		// Check bit starts at 0
		ck_assert_int_eq(bit_field_get(*b, 0), 0);
		// Set it to one, then check
		ck_assert_int_eq(bit_field_set(b, 0, 1), 0);
		ck_assert_int_eq(bit_field_get(*b, 0), 1);
		// Set it to zero, check again
		ck_assert_int_eq(bit_field_set(b, 0, 0), 0);
		ck_assert_int_eq(bit_field_get(*b, 0), 0);
	}
	bit_field_free(b);
	printf("\tFinished %s\n", __func__);
} END_TEST


START_TEST(bit_field_clear_test){
	const int SIZE = 100;
	BitField* b = bit_field_new(SIZE);

	// Set each bit to one
	for(int i = 0; i < SIZE; i++){
		ck_assert_int_eq(bit_field_set(b, i, 1), 0);
		ck_assert_int_eq(bit_field_get(*b, i), 1);
	}

	// Clear bits, then check they're all 0
	ck_assert_int_eq(bit_field_clear(b, SIZE), 0);
	for (int i = 0; i < SIZE; i++){
		ck_assert_int_eq(bit_field_get(*b, i), 0);
	}

	bit_field_free(b);
	printf("\tFinished %s\n", __func__);
} END_TEST


START_TEST(bit_field_memory_test){
	for(int i = 0; i < 100; i++){
		BitField* b = bit_field_new(i);
		for(int j = 0; j < i; j++){
			ck_assert_int_eq(bit_field_set(b, j, 1), 0);
		}
		bit_field_free(b);
	}
	printf("\tFinished %s\n", __func__);
} END_TEST


Suite* bitfield_suite(void){
	Suite* suite;
	TCase* core;

	suite = suite_create("BitField Suite");
	core = tcase_create("Core");
	tcase_add_test(core, bit_field_funcs_reject_null);
	tcase_add_test(core, bit_field_set_and_get);
	tcase_add_test(core, bit_field_clear_test);
	tcase_add_test(core, bit_field_memory_test);
	suite_add_tcase(suite, core);

	return suite;
}
#include <check.h>
#include <stdio.h>
#include "../src/memory.h"


// Inserts data, gets the pointer, then checks that the result
// from arena_index_from_pointer() matches the original index
START_TEST(arena_valid_pointer_test){
	Arena* a = arena_new(sizeof(uint32_t));
	for(int i = 0; i < ARENA_SIZE; i++){
		size_t o; size_t* output = &o;
		arena_insert(a, &output);
		void* loc = arena_get_data(a, *output);
		
		size_t c; size_t* compare = &c;
		arena_index_from_pointer(a, loc, &compare);
		
		ck_assert_int_eq(*compare, *output);
	}

	arena_free(&a);
	printf("\tFinished %s\n", __func__);
} END_TEST


START_TEST(arena_invalid_pointer_test){
	Arena* a = arena_new(sizeof(uint32_t));
	size_t o; size_t* output = &o;

	// NULL Arena Error
	ck_assert_int_eq(arena_index_from_pointer(NULL, NULL, &output), -1);
	
	// NULL output argument
	ck_assert_int_eq(arena_index_from_pointer(a, NULL, NULL), -2);
	
	// output argument points to NULL
	output = NULL;
	ck_assert_int_eq(arena_index_from_pointer(a, NULL, &output), -3);
	
	// pointer is below range
	output = &o;
	ck_assert_int_eq(arena_index_from_pointer(a, (void*)1, &output), -4);
	
	// pointer is above range
	output = &o;
	ck_assert_int_eq(arena_index_from_pointer(a, (void*)-1, &output), -5);
	
	// pointer is not at a valid offset
	output = &o;
	arena_insert(a, &output);
	void* loc = arena_get_data(a, *output);
	ck_assert_int_eq(arena_index_from_pointer(a, ((uint8_t*)loc)+1, &output), -6);

	// pointer is one below minimum
	output = &o;
	ck_assert_int_eq(arena_index_from_pointer(a, ((char*)(a->data))-1, &output), -4);

	// pointer is one above maximum
	output = &o;
	ck_assert_int_eq(arena_index_from_pointer(a, ((char*)(a->data))+(a->max_items*a->data_size)+1, &output), -5);

	arena_free(&a);
	printf("\tFinished %s\n", __func__);
} END_TEST


Suite* arena_suite(void){
	Suite* suite;
	TCase* core;

	suite = suite_create("Arena Suite");
	core = tcase_create("Core");
	tcase_add_test(core, arena_valid_pointer_test);
	tcase_add_test(core, arena_invalid_pointer_test);
	suite_add_tcase(suite, core);

	return suite;
}
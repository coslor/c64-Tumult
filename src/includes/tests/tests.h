#pragma once

#pragma message("***in tests.h")

#include <stdint.h>

#define MAX_TEST_ARGS 4

typedef const bool (*Test_Ptr)(char * name,int num_args, const uint32_t args[MAX_TEST_ARGS]);

typedef struct  {
	char* description;
//	const void (*test_func_ptr)(char *,int num_args, uint32_t args[MAX_TEST_ARGS]);
//	Test_Ptr test_func_ptr;
	void  *test_func_ptr;
	const int num_args;
	uint32_t args[MAX_TEST_ARGS];
} Test;

void run_tests(char *report_title, Test tests[], int num_tests);

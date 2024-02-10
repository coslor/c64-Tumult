#include <stdbool.h>
#include "../includes/tests/tests.h"
#include <stdio.h>
#include <stddef.h>

//#define Test_Entry void *

//void run_tests(char *report_title, Test tests[], int num_tests);


void run_tests(char *report_title, Test tests[], int num_tests) {

	printf("%s\n\n", report_title);

	int num_passed=0;

	for (int i=0;i<num_tests;i++) {
		Test_Ptr p = tests[i].test_func_ptr;
		bool result = p(tests[i].description,tests[i].args, tests[i].num_args);
		if (result) { num_passed++;}

		printf("Test #%d(%s) result: %s\n", i, tests[i].description, result ? "pass":"fail");
	}
	printf("Tests complete! %d tests passed\n", num_passed);
}


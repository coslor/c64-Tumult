#include <stdint.h>

#define MAX_TEST_ARGS 4

typedef bool (*Test_Ptr)(char *,int num_args, uint32_t args[MAX_TEST_ARGS]);

typedef struct  {
	char* description;
	const Test_Ptr test_func_ptr;
	const int num_args;
	uint32_t args[MAX_TEST_ARGS];
} Test;

void run_tests(char *report_title, Test tests[], int num_tests);

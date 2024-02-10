#pragma message("***in test_logger.c")

//#define CLC_LOGGING "1"
#ifdef CLC_LOGGING
	#pragma message("CLC_LOGGING is defined")
#else
	#pragma message("CLC_LOGGING is undefined")
#endif

#include "../includes/logger.h"
#define LOG_LEVEL ERROR_LEVEL

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <conio.h>
#include <stdio.h>

#include "../includes/tests/tests.h"
#include "../includes/tests/test_logger.h"
#include "../includes/logger.h"



void print_log_messages() {
	log_debug(	"DEBUG message\n");
	log_info(	"INFO message\n");
	log_warn(	"WARN message\n");
	log_error(	"ERROR message\n");
}

int main(void) {

	// static const char desc[] = "Test Tests";
	// static const char test_arg1[] = "TEST1";
	// //static const uint32_t int_str=(const uint32_t)test_arg1;

	// static const char * first_test_name="First Test";

	// //entries[0]=entry;

	// uint32_t test_name_num=(uint32_t)"TEST1";


	// printf("running test...\n");



	// //NOTE: embedding the data all to gether like this is the ONLY FUCKING
	// //		way I could make this work, due to C's obscure scoping rules
	// run_tests(
	// 	"1-Test Report", 
	// 	//tests,
	// 	1, 
	// 	(Test[]) {		//P.S. aren't "Test[]" and "Test *" supposed to be the same damn thing in c???? 'Cause the star notation doesn't compile
	// 		{  
	// 			.description = NULL, //"Test Tests",
	// 			.test_func_ptr = test_log_levels,
	// 			.num_args=1,
	// 			.args={0}
	// 		}	
	// 	}, 
	// 	0);

	// return 0;

	iocharmap(IOCHM_PETSCII_2);	//put printf() in lowercase mode

	const uint32_t args[] = {0,0,0,0};

	bool success = test_log_levels((char *)"Log Levels", 1, args);
	printf("Result:%s",success?"SUCCESS":"FAILURE");
	return (success ? 0 : -1);
}

const bool test_log_levels(char *description,int num_args, const uint32_t args[MAX_TEST_ARGS]){
	printf("Test %s\n", description);

	set_log_level(DEBUG_LEVEL);
	printf("---DEBUG-LEVEL:%s---\n", get_log_level_name());
	print_log_messages();

	set_log_level(INFO_LEVEL);
	printf("---INFO-LEVEL:%s---\n", get_log_level_name());
	print_log_messages();

	set_log_level(WARN_LEVEL);
	printf("---WARN-LEVEL:%s---\n", get_log_level_name());
	print_log_messages();

	set_log_level(ERROR_LEVEL);
	printf("---ERROR-LEVEL:%s---\n", get_log_level_name());
	print_log_messages();

	printf("Do the logs look right (y/n)");
	char c=getchar();
	printf("\n");
	return (c == 'y');
}


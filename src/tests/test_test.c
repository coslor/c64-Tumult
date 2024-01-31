#include "../includes/tests/tests.h"
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <c64/types.h>
#include <stdarg.h>


//void run_tests(char *title, Test_Entry tests[], int num_tests)

// typedef void *va_list;

// #define	va_start(list, name) (void) (list = &name + 1)

// #define	va_arg(list, mode) ((mode *)(list = (char *)list + sizeof (mode)))[-1]

// #define	va_end(list) (void)0

const bool test_test_func(char *,int, uint32_t[]);

//static const char *test1_desc="test 1";

static const char TEST1[]="test1";

const Test test1=	{  
	.description = 0, //"Test Tests",
	.test_func_ptr = test_test_func,
	.num_args=3,
	.args={
		(uint32_t)0,//"TEST1"
		(uint32_t)3.0,
		(uint32_t)-17
	}
};





int main(void) {

	static const char desc[] = "Test Tests";
	static const char test_arg1[] = "TEST1";
	//static const uint32_t int_str=(const uint32_t)test_arg1;

	static const char * first_test_name="First Test";

	//entries[0]=entry;

	uint32_t test_name_num=(uint32_t)"TEST1";


	printf("running test...\n");


// 	Test tests[] = { 
// //		test1 };
// 		{  
// 			.description = "Test Tests",
// 			.test_func_ptr = test_test_func,
// 			.num_args=3,
// 			.args={
// 				int_str,
// 				(uint32_t)3.0,
// 				(uint32_t)-17
// 			}
// 		}
// 	};

	//NOTE: embedding the data all to gether like this is the ONLY FUCKING
	//		way I could make this work, due to C's obscure scoping rules
	//run_tests(
	// 	"1-Test Report", 
	// 	tests,
	// 	// 1, 
	// 	// (Test[]) {		//P.S. aren't "Test[]" and "Test *" supposed to be the same damn thing in c???? 'Cause the star notation doesn't compile
	// 	// 	{  
	// 	// 		.description = 0, //"Test Tests",
	// 	// 		.test_func_ptr = test_test_func,
	// 	// 		.num_args=3,
	// 	// 		.args={
	// 	// 			(uint32_t)0,//"TEST1"
	// 	// 			(uint32_t)3.0,
	// 	// 			(uint32_t)-17
	// 	// 		}
	// 	// 	}	
	// 	// }, 
	// 	3);

	return 0;

}


const bool test_test_func(char *description,int num_args, uint32_t args[]) {
	if (num_args != 3) {
		printf("\nError: test needs 3 args, only got %d", num_args);
		return false;
	}

	printf("test got args: %f,%d\n", (char *)args[1], (float)args[2]);
	return true;
}


#include <stdio.h>

	typedef struct Test { 
		int a;
		int b;
	} Test;

	Test test = {.b=1,.a=2};
	

int main() {
	printf(p"test=%d,%d\n", test.a,test.b);
	return 0;
}
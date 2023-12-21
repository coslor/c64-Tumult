#include <stdio.h>

#define DEBUG

char * sformat(char *, const char*, int *, bool);
void out(const char *, ...);

int main(void)
	{
		out("Hello log world\n");
		out("Hello %s\n", "Chris");
		return 1;
	}

void out(const char *fmt, ...)
	{
#ifdef DEBUG 				
	char buff[50];
	//const char *fmt = "0123456789012345678901234567890123456789012345678901234567890123456789";
	sformat(buff, fmt, (int *)&fmt + 1, true);
#endif
}

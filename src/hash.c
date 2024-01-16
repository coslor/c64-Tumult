#include <stdio.h>
#include <conio.h>
#include <stdint.h>
#include <string.h>

/*
 *	MurmurHash, an apparently very efficient hashing function for strings.
 *
*/
uint32_t MurmurOAAT_32(const char* str, int h)
{
    // One-byte-at-a-time hash based on Murmur's mix
    // Source: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp
    for (; *str; ++str) {
        h ^= *str;
        h *= (int)0x5bd1e995;
        h ^= h >> 15;
    }
    return h;
}

int main(void) {
	char buffer[88];
	long hash;

	iocharmap(IOCHM_PETSCII_2);	//put printf() in lowercase mode

	
	do {
		printf("Enter name:");
		gets(buffer);
		long hash=MurmurOAAT_32(buffer,hash);
		printf ("\nBuffer hash is %lx\n",hash);

	} while(strlen(buffer)>0);

	printf("bye!");
	return 0;
}
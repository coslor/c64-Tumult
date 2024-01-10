#include <stddef.h>
#include <c64/cia.h>
#include <stdbool.h>
#include <stdint.h>

struct key_entry {
	char key;
	byte column;
	byte row;
};


bool check_for_key(byte column, byte row);
bool check_for_key_char(char key);
static key_entry *get_key_entry(char key);

static const struct key_entry key_matrix[] = {
	{3, 	0b01111111,	0b10000000},
	{'w',	0b11111101,	0b00000010},
	{'a',	0b11111101,	0b00000100},
	{'s', 	0b11111101, 0b00100000},
	{'d',	0b11111011, 0b00000100},
	{' ',	0b01111111,	0b00010000}
};

#pragma compile("/../keyboard.c")

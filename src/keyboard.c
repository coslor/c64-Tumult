#include "includes/keyboard.h"

bool check_for_key(byte column, byte row) {
	__asm {sei};

	cia1.ddra 	= 0b11111111;	//DDR A = all outputs
	cia1.ddrb 	= 0b00000000;	//DDR B = all inputs

	cia1.pra	= column;

	byte val = (cia1.prb & row);
	__asm {cli};
	return (val == 0);
}

/*
*	Returns the key_entry (CIA keyboard row & col values) for the given key (ex: 's' for the S key).
*/
static key_entry *get_key_entry(char key) {
	for (int i=0;i<(sizeof(key_matrix) / 3); i++) {
		if (key == key_matrix[i].key) {
			return &key_matrix[i];
		}
	}
	return NULL;
}

bool check_for_key_char(char key) {
	key_entry *entry = get_key_entry(key);
	if (entry != NULL) {
		return check_for_key(entry->column, entry->row);
	}
	else {
		printf("***CHECK-FOR-KEY-CHAR():KEY %c NOT FOUND\n",key);
	}
	return false;
}
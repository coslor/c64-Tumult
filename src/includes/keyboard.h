#pragma once


#include <stddef.h>
#include <c64/cia.h>
#include <stdbool.h>
#include <stdint.h>

#include <c64/types.h>

struct cia_matrix {
	byte column;
	byte row;
};

static struct key_entry {
	char key;
	struct cia_matrix matrix;
};

#ifndef CIA_MATRIX_DEFINED
#define CIA_MATRIX_DEFINED 1

static struct cia_matrix up_matrix		={ 0b11111101,	0b00000010};	//default=W
static struct cia_matrix left_matrix 	={ 0b11111101,	0b00000100};	//default=A
static struct cia_matrix right_matrix	={ 0b11111101, 	0b00100000};	//default=D
static struct cia_matrix down_matrix	={ 0b11111011, 	0b00000100};	//default=S
static struct cia_matrix fire_matrix	={ 0b01111111,	0b00010000};	//default=SPACE
static struct cia_matrix esc_matrix	={ 0b01111111,	0b10000000};	//default=RUN/STOP

#endif

bool check_for_matrix(struct cia_matrix matrix);
bool check_for_key(byte, byte);
bool check_for_key_char(char);


/*
*	Returns the key_entry (CIA keyboard row & col values) for the given key (ex: 's' for the S key).
*/
static key_entry *get_key_entry(char key);

static const struct key_entry key_matrix[] = {
	{3, 	{ 0b01111111,	0b10000000} },
	{'w',	{ 0b11111101,	0b00000010} },
	{'a',	{ 0b11111101,	0b00000100} },
	{'s', 	{ 0b11111101, 	0b00100000} },
	{'d',	{ 0b11111011, 	0b00000100} },
	{' ',	{ 0b01111111,	0b00010000} }
};

//#pragma compile("/../keyboard.c");
//#endif


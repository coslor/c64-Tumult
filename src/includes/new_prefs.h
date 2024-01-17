#pragma once

#include <stdbool.h>
#include <c64/types.h>
#include <c64/kernalio.h>

typedef struct New_Prefs {
	byte		input_device;
	byte		prefs_device;

	byte 		up_key;
	byte		left_key;
	byte		down_key;
	byte 		right_key;
	byte		fire_key;
	byte		menu_key;
} New_Prefs;

bool save_prefs(New_Prefs *prefs, byte file_num, byte dev_num, char *filename);
bool load_prefs(New_Prefs *prefs, byte file_num, byte dev_num, char *filename);
void clear_prefs(New_Prefs *prefs);
void copy_prefs(New_Prefs *dest, New_Prefs *source);
void init_prefs(New_Prefs *dest_prefs, New_Prefs new_prefs);
void print_prefs(New_Prefs *prefs);
				

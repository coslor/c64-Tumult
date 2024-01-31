#pragma once

#include <c64/types.h>

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

bool save_prefs(New_Prefs *prefs, byte file_num, byte channel_num, const char *filename);
bool load_prefs(New_Prefs *prefs, byte file_num, byte channel_num, const char *filename);
void clear_prefs(New_Prefs *prefs);
void copy_prefs(New_Prefs *dest, New_Prefs *source);
void init_prefs(New_Prefs *dest_prefs, New_Prefs new_prefs);
void print_prefs(New_Prefs *prefs);
void print_dir(int dev_num);
bool open_error_channel(int file_num, int device_num);
void close_error_channel(int file_num);

/**
 * Checks channel 15 on the drive for the given file num for errors. Assumes that the drive channel
 * 	has already been opened, and does **NOT** close it upon return.
 * 
 * @param file_num 	
 * @param buffer 			The full error line if error >= 0
 * @param max_buffer_len 	The max length of the buffer
 * 
 * @return int 			Returns 0 if no error, 
 * 						a positive drive error # if the drive does have an error, 
 * 						or a negative error if we were unable to contact the drive properly.
 */
int check_for_drive_error(int file_num, char *buffer, int max_buffer_len);				

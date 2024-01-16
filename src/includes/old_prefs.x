#pragma once
#ifndef PREFS_H
#define PREFS_H

#include <stdint.h>
#include <c64/types.h>
#include <c64/keyboard.h>
#include <stdbool.h>
#include <c64/kernalio.h>


#define PREFS_FILE_NAME 		"Tumult prefs.seq"
#define PREFS_FILE_NUM			1
#define DISK_STATUS_FILE_NUM	15	

#define MAX_PREF_NAME_LEN		40
#define MAX_PREFS_ENTRIES		16

union Prefs_Value {
	char 					str_value[];
	int 					int_value;
};

typedef struct Prefs_Entry {
	const char 				name[MAX_PREF_NAME_LEN];
	union Prefs_Value		value;
} Tumult_Pref_Entry;

struct Preferences {
	int 					input_device;
	int 					prefs_device;
	struct Prefs_Entry		entries[];
	int						entry_count;
};

struct Tumult_Prefs {
	// The "superclass" object
	struct Preferences 		base_prefs;

	int up_key;
	int left_key;
	int right_key;
	int down_key;
	int fire_key;
	int stop_key;
};


//#define UND=0xe4;

// //NOTE: 
// typedef enum Tumult_Pref_Name {
// 	PREFS_INPUT_DEVICE_NUM,
// 	PREFS_PREFS_DEVICE_NUM,
// 	PREFS_UP_KEY,
// 	PREFS_DOWN_KEY,
// 	PREFS_LEFT_KEY,
// 	PREFS_RIGHT_KEY,
// 	PREFS_FIRE_KEY,
// 	PREFS_STOP_KEY
// } Tumult_Pref_Name;


#define NUM_TUMULT_PREF_ENTRIES 8

// //NOTE: "\\xe4"==PETSCII underscore. I have no idea why oscar xlates "_" to the PETSCII back-arrow character, but this one is correct
// Tumult_Pref_Entry TUMULT_PREF_ENTRIES[] = {
// 	{ PREFS_INPUT_DEVICE_NUM,	"input\xe4device"},
// 	{ PREFS_PREFS_DEVICE_NUM,	"prefs\xe4device"},
// 	{ PREFS_UP_KEY,				"prefs\xe4up\xe4key"},
// 	{ PREFS_DOWN_KEY, 			"prefs\xe4down\xe4key"},
// 	{ PREFS_LEFT_KEY, 			"prefs\xe4left\xe4key"},
// 	{ PREFS_RIGHT_KEY,			"prefs\xe4fire\xe4key"},
// 	{ PREFS_FIRE_KEY,			"prefs\xe4fire\xe4key"},
// 	{ PREFS_STOP_KEY,			"prefs\xe4stop\xe4key"}
// };

// char [][] PREFERENCES_ENTRY_STRING = {
// 	"input\xe4device",
// }
#define PREFS_INPUT_DEVICE_NUM	
#define	PREFS_PREFS_DEVICE_NUM	"prefs\xe4device"
#define PREFS_UP_KEY			"prefs\xe4up\xe4key"
#define PREFS_DOWN_KEY			"prefs\xe4down\xe4key"
#define PREFS_LEFT_KEY			"prefs\xe4left\xe4key"
#define PREFS_RIGHT_KEY			"prefs\xe4right\xe4key"
#define PREFS_FIRE_KEY			"prefs\xe4fire\xe4key"
#define PREFS_STOP_KEY			"prefs\xe4stop\xe4key"


/**
 * @brief 	OPEN file_num,dev_num,0,filename; READ file_num INTO prefs; close_prefs_file()
 * 
 * @param prefs 	Ptr to the object
 * @param dev_num 	
 * @param file_num 
 * @param filename 
 * @return false 	if the read fails
 */
bool read_prefs(struct Tumult_Prefs *prefs, byte dev_num, byte file_num, char *filename);

/**
 * @brief Calls a Kernal close() on file_num, and prints an error if STatus is not 64 or 0
 * 
 * @param file_num 
 */
void close_prefs_file(byte file_num);

static void process_prefs_line(Tumult_Prefs p,char *line, int line_len);
static bool process_prefs_entry(struct Tumult_Prefs p, char *prop_name, char *prop_value);
bool write_prefs(char *filename);
void print_prefs(struct Tumult_Prefs prefs);
void close_prefs_file(byte file_num);


#endif

#pragma once
#include <stdint.h>

#define PREFS_FILE_NAME 		"Tumult prefs.seq"
#define PREFS_FILE_NUM			1
#define DISK_STATUS_FILE_NUM	15	

#define MAX_PREF_NAME_LEN		40
#define MAX_PREFS_ENTRIES		16

#define PREFS_PRINTF_COUNT_STR	"%d entries\n"
#define PREFS_PRINTF_ENTRY_STR	"%d:%s=%d\n"

typedef union Prefs_Value {
	char* 					str_value;
	int 					int_value;
} Prefs_Value;

/**
 * A representation of a specific preference for a program.
 * 
 * A 
 * ```
 * Preference
 * ..........^ 
 * ```
 * holds a list of 
 * ```
 * Preferences. 
 * ..........^
 * ```
 * 
 * See the the difference?
 * 
 */
typedef struct Preference {
	char	 				name[MAX_PREF_NAME_LEN];
	int						value;
} Preference;

/**
  A list of preference entries with their names & values. 
  
  The typical paradigm for creating the enum should be e.g.:

  typedef enum TumultPrefName {
	PREFS_INPUT_DEVICE_NUM,
  };
  	Preferences p;
  	p.entries[PREFS_INPUT_DEVICE_NUM] 	= (Prefs_Entry){.name="input_device", 	.value=2};
   
  ...and accessing the values should be: 

    char *string_value=(char *)(prefs.entries[PREFS_INPUT_DEVICE_NUM].value);

 You can also access the values by preference name, e.g.:
 
 **/
typedef struct Preferences {
	//int 					input_device;
	//int 					prefs_device;
	struct Preference		*entries;
	int						entry_count;
} Preferences;

void set_pref_value(Preferences *prefs, int pref_num, const char *pref_name, int pref_value);
Preference find_preference_by_name(Preferences *prefs, const char *pref_name);
int find_pref_value_by_name(Preferences *prefs, const char *pref_name);
void print_prefs(Preferences *prefs);
bool read_prefs(Preferences *prefs, int enum_count);
bool read_prefs_from_file(int file_num, const char *filename, Preferences *prefs);
bool write_prefs_to_file(Preferences *prefs, int file_num, const char *filename) ;

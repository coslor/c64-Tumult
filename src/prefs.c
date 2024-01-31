#include "includes/prefs.h"
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <c64/kernalio.h>
#include "includes/tumult_prefs.h"
#include <stdbool.h>

/**
 * @brief A list of preference entries with their names & values. The typical paradigm for accessing the values
 * 			should be: 
 * 			~~~
 * 			char *string_value=prefs.entries[]
 * 
 */

//static int pref_entry_count=0;

/**
 * @brief Sets the values for a preference at a given index.
 * 			NOTE: 	The pref name string is COPIED, so it's OK to pass in stack variables here.
 * 
 * @param prefs 		The Preferences object
 * @param pref_num 		Index of the entry we want
 * @param pref_name 
 * @param pref_value 	Should be of type int or const char*
 */
void set_pref_value(Preferences *prefs, int pref_num, const char *pref_name, int pref_value) {
	strcpy(prefs->entries[pref_num].name, pref_name);
	prefs->entries[pref_num].value = pref_value;
}

Preference find_preference_by_name(Preferences *prefs, const char *pref_name) {
	for (int i=0; i < prefs->entry_count; i++) {
		if (strcmp(prefs->entries[i].name, pref_name) == 0) {
			return prefs->entries[i];
		}
	}
	return NULL;
}

/**
 * @brief Find the preference entry value for the given string. 
 * 			NOTE: this shouldn't be the typical way of accessing the prefs values. Rather, use the paradigm in the prefs definition above.
 * 
 * @param prefs 
 * @param pref_name 
 * @return int 
 */
 int find_pref_value_by_name(Preferences *prefs, const char *pref_name) {
	Preference pref = find_preference_by_name(prefs, pref_name);
	if (pref != NULL) {
		return pref.value;
	}
	else {
		return NULL;
	}
 }

void print_prefs(Preferences *prefs) {
	printf(PREFS_PRINTF_COUNT_STR, prefs->entry_count);
	for (int i=0;i<prefs->entry_count;i++) {
		printf(PREFS_PRINTF_ENTRY_STR,i,prefs->entries[i].name, prefs->entries[i].value);
	}
}

/*
	NOTE: this routine uses prefs->device_num, so it must be set to a nonzero value
*/

bool write_prefs_to_file(Preferences *prefs, int file_num, const char *filename) {
	static const char printf_buffer[80];
	static const char file_buffer[80];

	bool success;
	int dev_num=prefs->entries[PREFS_PREFS_DEVICE_NUM].value;

	if (dev_num == 0) {
		printf("preferences.device_num must be set!\n");
		return false;
	}

	printf("Setting name...\n");
	krnio_setnam(filename);

	printf("opening file %d,%d,%d...\n", file_num, dev_num, 1);
	if (success = krnio_open(file_num, dev_num,1)) {

		printf("calling chkout...\n");
		if (success = krnio_chkout(file_num)) {
			print_prefs(prefs);
			success = ((krnio_pstatus[dev_num] & 0b10111111) ==0);
		} else {
			//print_kernalio_message(dev_num,"error calling chkout()\n");
		}
		//krnio_close(file_num);
		//TODO something weird is going on here!!
		krnio_clrchn();
		printf("file closed!\n");
		if (!success) {
			print_kernalio_message(dev_num,"Error printing preferences");
		}
	}
	return success;
}

/*
	NOTE: this routine doesn't try to cross-check the enums. If the enums from the current Preferences is 
			different from the one being read, well, bad things may happen. That's on you.

*/
bool read_prefs(Preferences *prefs) {
	static char scanf_buffer[80];
	static char file_buffer[80];

	int line_len;

	//First, skip any blank lines, or lines starting with "#"
	//NOTE: krnio_gets() calls chkin() before and clrchn() after the read
	do {
		gets(&file_buffer);
	} while ((line_len==0) || (file_buffer[0]=='#'));

	//We have a line that isn't either blank or a comment, so we don't want to do another read
	//gets(file_buffer);
	if (scanf(PREFS_PRINTF_COUNT_STR, &(prefs->entry_count)) == 0) {
		printf("Error in read_prefs():unable to read enum count");
		return false;
	}

	// if (count_read != enum_count) {
	// 	printf("Error in read_prefs(): prefs count received %d differs from enum count %d", count_read, enum_count);
	// 	return false;
	// }

	for (int i=0;i<(prefs->entry_count);i++) {
		int pref_num;
		char *pref_name;
		int pref_value;
		//Preference p
		gets(&scanf_buffer);
		scanf(PREFS_PRINTF_ENTRY_STR, &pref_num, pref_name, &pref_value);
		if (pref_num != i) {
			printf("Error in read_prefs(): preference #%d != %d\n", pref_num, i);
			return false;
		}
		set_pref_value(prefs,pref_num, pref_name, pref_value);
	}
	return true;
}


/**
 * @brief 				Opens the file, calls CHKIN, and then calls read_prefs()
 * 
 * @param file_num 		file handle #
 * @param dev_num 		device #(ex. 8)
 * @param filename 
 * @param prefs 
 */
bool read_prefs_from_file(int file_num, const char *filename, Preferences *prefs) {

	static const char SCANF_BUFFER[80];
	static const char file_buffer[80];
	int dev_num=prefs->entries[PREFS_PREFS_DEVICE_NUM].value;

	bool success;

	if (dev_num == 0) {
		printf("device number should not be 0\n");
		return false;
	};

	krnio_setnam(filename);
	if ((success = krnio_open(file_num,dev_num,0) > 0)) {
		if (success = krnio_chkin(file_num)) {
			if (success = read_prefs(prefs) && ((krnio_pstatus[dev_num]) &0b10111111) == 0) {
				printf("prefs file successfully read!\n");
			} else {
				print_kernalio_message(dev_num,"error reading prefs from file  \"%s\"", filename);
			}
		} else {
			print_kernalio_message(dev_num,"error setting input file for CHKIN()");
		}
		krnio_close(file_num);
		return success;
	} else {
		print_kernalio_message(dev_num,"unable to open file %d on device %d", file_num, dev_num);
	}
	return success;
}


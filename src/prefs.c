#include "includes/prefs.h"
#include <string.h>
#include <stddef.h>
#include <stdio.h>

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
void set_pref_value(Preferences *prefs, int pref_num, char *pref_name, int pref_value) {
	strcmp(prefs->entries[pref_num].name, pref_name);
	prefs->entries[pref_num].value = pref_value;
}

Preference find_preference_by_name(Preferences *prefs, char *pref_name) {
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
 int find_pref_value_by_name(Preferences *prefs, char *pref_name) {
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
	NOTE: this routine doesn't try to cross-check the enums. If the enums from the current Preferences is 
			different from the one being read, well, bad things may happen. That's on you.
*/
bool read_prefs(Preferences *prefs, int enum_count) {
	int count_read;
	if (scanf(PREFS_PRINTF_COUNT_STR, &count_read) == 0) {
		printf("Error in read_prefs():unable to read enum count");
		return false;
	}

	if (count_read != enum_count) {
		printf("Error in read_prefs(): prefs count received %d differs from enum count %d", count_read, enum_count);
		return false;
	}

	for (int i=0;i<prefs->entry_count;i++) {
		int pref_num;
		char *pref_name;
		int pref_value;
		scanf(PREFS_PRINTF_ENTRY_STR, &pref_num, pref_name, &pref_value);
		if (pref_num != i) {
			printf("Error in read_prefs(): preference #%d != %d\n", pref_num, i);
			return false;
		}
		set_pref_value(prefs,pref_num, pref_name, pref_value);
	}
	return true;
}
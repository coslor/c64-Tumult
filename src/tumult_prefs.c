#include "includes/tumult_prefs.h"

extern Preferences prefs;


void init_tumult_prefs() {
	prefs.entry_count = 8;
	prefs.entries = new Preference[prefs.entry_count];

	printf("Setting entries\n");
	prefs.entries[PREFS_INPUT_DEVICE_NUM] 	= (Preference){.name="input_device", 	.value=2};
	prefs.entries[PREFS_PREFS_DEVICE_NUM] 	= (Preference){.name="prefs_device", 	.value=0};
	prefs.entries[PREFS_UP_KEY] 			= (Preference){.name="up_key",	 		.value=KSCAN_I};
	prefs.entries[PREFS_DOWN_KEY]			= (Preference){.name="down_key", 		.value=KSCAN_K};
	prefs.entries[PREFS_LEFT_KEY]			= (Preference){.name="left_key", 		.value=KSCAN_J};
	prefs.entries[PREFS_RIGHT_KEY]			= (Preference){.name="down_key", 		.value=KSCAN_L};
	prefs.entries[PREFS_FIRE_KEY]			= (Preference){.name="fire_key", 		.value=KSCAN_SPACE};
	prefs.entries[PREFS_STOP_KEY]			= (Preference){.name="stop_key", 		.value=KSCAN_STOP};

	//We have just as many Prefs_Entries as we have enums
	//prefs.entry_count = sizeof(Tumult_Pref_Name);
}


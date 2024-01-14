#include <c64/keyboard.h>
#include "prefs.h"

typedef enum Tumult_Pref_Name {
	PREFS_INPUT_DEVICE_NUM,
	PREFS_PREFS_DEVICE_NUM,
	PREFS_UP_KEY,
	PREFS_DOWN_KEY,
	PREFS_LEFT_KEY,
	PREFS_RIGHT_KEY,
	PREFS_FIRE_KEY,
	PREFS_STOP_KEY
} Tumult_Pref_Name;

void init_tumult_prefs();

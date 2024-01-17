#include "includes/new_prefs.h"
#include "includes/Tumult.h"
#include <stdbool.h>
//extern Preferences prefs;
extern struct New_Prefs	prefs;


// void close_prefs_file(byte file_num) ;

void init_tumult_prefs() {

	// int a[5] = {[4]=1,[3]=2,[2]=3,[1]=4,[0]=5};

	// typedef struct Test { 
	// 	int a; 
	// 	int b; 
	// } Test;

	// Test test1 = {1,2};

	New_Prefs empty_prefs = (New_Prefs){ 
		//2,LAST_DEVICE_NUM,KSCAN_I,KSCAN_J,KSCAN_K,KSCAN_L,KSCAN_SPACE, KSCAN_STOP};
		.input_device=2,
		// .prefs_device = (const byte)(LAST_DEVICE_NUM),
		.up_key = KSCAN_I, 
		.left_key = KSCAN_J, 
		.right_key = KSCAN_L,
		.down_key = KSCAN_K,
		.fire_key = KSCAN_SPACE, 
		.menu_key = KSCAN_STOP 
		 };
	empty_prefs.prefs_device = LAST_DEVICE_NUM;

	init_prefs(&prefs, empty_prefs);

	// prefs.prefs_device 	= LAST_DEVICE_NUM;
	// prefs.input_device	= 2;
	// prefs.up_key		= KSCAN_I;
	// prefs.left_key		= KSCAN_J;
	// prefs.
	// printf("Setting entries\n");
	// prefs.entries[PREFS_INPUT_DEVICE_NUM] 	= (Preference){.name="input_device", 	.value=2};
	// int dev_num=LAST_DEVICE_NUM;
	// //prefs.entries[PREFS_PREFS_DEVICE_NUM] 	= (Preference){.name="prefs_device", 	.value=(const byte)*((byte *)0xba)};
	// set_pref_value(&prefs,PREFS_PREFS_DEVICE_NUM, 	"prefs_device", (int)LAST_DEVICE_NUM);
	// //prefs.entries[PREFS_UP_KEY] 			= (Preference){.name="up_key",	 		.value=KSCAN_I};
	// set_pref_value(&prefs,PREFS_UP_KEY, 			"up_key", 		KSCAN_I);
	// //prefs.entries[PREFS_DOWN_KEY]			= (Preference){.name="down_key", 		.value=KSCAN_K};
	// set_pref_value(&prefs,PREFS_DOWN_KEY, 			"down_key", 	KSCAN_K);
	// //prefs.entries[PREFS_LEFT_KEY]			= (Preference){.name="left_key", 		.value=KSCAN_J};
	// set_pref_value(&prefs,PREFS_LEFT_KEY, 			"left_key", 	KSCAN_J);
	// //prefs.entries[PREFS_RIGHT_KEY]			= (Preference){.name="down_key", 		.value=KSCAN_L};
	// set_pref_value(&prefs,PREFS_RIGHT_KEY, 			"right_key", 	KSCAN_L);
	// //prefs.entries[PREFS_FIRE_KEY]			= (Preference){.name="fire_key", 		.value=KSCAN_SPACE};
	// set_pref_value(&prefs,PREFS_FIRE_KEY, 			"fire_key", 	KSCAN_SPACE);
	// //prefs.entries[PREFS_STOP_KEY]			= (Preference){.name="stop_key", 		.value=KSCAN_STOP};
	// set_pref_value(&prefs,PREFS_STOP_KEY, 			"stop_key", 	KSCAN_STOP);

	//We have just as many Prefs_Entries as we have enums
	//prefs.entry_count = sizeof(Tumult_Pref_Name);

	//set_pref_value(&prefs,PREFS_PREFS_DEVICE_NUM, "prefs_device", (int)LAST_DEVICE_NUM);
}

// /**********************prefs*********************************************************************/
// /* open prefs file, reads the data in, setting preferences as necessary, and closes the file */
// bool read_prefs(Preferences *prefs, Preference entries[], byte dev_num, byte file_num, char *filename) {
// 	static const char SCANF_BUFFER[80];
// 	static const char FILE_BUFFER[80];

// 	if (prefs->prefs_device == 0) {
// 		printf("device number should not be 0\n");
// 		return false;
// 	};

// 	krnio_setnam(filename);
// 	if (krnio_open(file_num,dev_num,0)) {
// 		bool keep_reading = true;
// 		while (keep_reading) {
// 			int line_len=0;
// 			//First, skip any blank lines, or lines starting with "#"
// 			//NOTE: krnio_gets() calls chkin() before and clrchn() after the read
// 			do {
// 				line_len = krnio_gets(PREFS_FILE_NUM,FILE_BUFFER,80);
// 			} while ((line_len==0) || (FILE_BUFFER[0]=='#'));

// 	// 		sprintf(SCANF_BUFFER, );
// 	// printf("%s=%d\n", PREFS_INPUT_DEVICE_NUM,	prefs.input_device);
// 	// printf("%s=%d\n", PREFS_PREFS_DEVICE_NUM,	prefs.prefs_device);
// 	// printf("%s=%d\n", PREFS_UP_KEY, 			prefs.up_key);
// 	// printf("%s=%d\n", PREFS_DOWN_KEY, 			prefs.down_key);
// 	// printf("%s=%d\n", PREFS_LEFT_KEY,  			prefs.left_key);
// 	// printf("%s=%d\n", PREFS_RIGHT_KEY,			prefs.right_key);
// 	// printf("%s=%d\n", PREFS_FIRE_KEY, 			prefs.fire_key);




// 			if (line_len == 0) {
// 				printf("read_prefs() finished with EOL\n");
// 				keep_reading = false;
// 			} else {
// 				printf("ERROR read_prefs: error %d reading lines\n",line_len);
// 				keep_reading = false;
// 			}

// 		}
// 		byte status = krnio_pstatus[file_num];
// 		close_file(PREFS_FILE_NUM);

// 		if (status & 0b10111111) {			//are any bits set in the status word except for EOF?
// 			print_kernalio_message(file_num, "We're out of data, but EOF isn't set?!?");
// 			return false;
// 		}
// 		else {
// 			printf("Prefs file read successfully!\n");
// 			return true;
// 		}
// 	//}
// 		// } else {
// 		// 	//sprintf(kernalio_buffer, "Unable to process line for file %s\n", PREFS_FILE_NAME);
// 		// 	print_kernalio_message(1, "Unable to process line in file %s\n", PREFS_FILE_NAME);

// 		// 	close_file(PREFS_FILE_NUM);
// 		// 	return false;
// 		// }

// 	} else {
// 		//sprintf(kernalio_buffer, "Unable to open file %s\n", PREFS_FILE_NAME);
// 		print_kernalio_message(dev_num, "Unable to open file %s\n", filename);
// 		close_prefs_file(file_num);
// 		return false;
// 	}
// }


// /**
//  * @brief Close the given file number, and print a warning if there's an error associated with it.
//  * 
//  * @param file_num 
//  */
// void close_prefs_file(byte file_num) {
// 	krnio_close(file_num);
// 	if (krnio_pstatus[file_num] & 0b10111111) {	//is the status anything but EOF?
// 			// sprintf(kernalio_buffer, "File read finished, but there's a nonzero status");
// 			print_kernalio_message(file_num,"File read finished, but there's a nonzero status");
// 	}
		
// };

// /**
//  * @brief Read a prefs line
//  * 
//  * @param line 
//  * @param prop_name 
//  */
// bool process_prefs_line(Preferences *p,char *line, int line_len) {
// 	byte n=0;
// 	byte pn_len=0;

// 	byte pv_len=0;

// 	int prop_value;
// 	//char *prop_name;

// 	static char prop_name[80];
// 	static char prop_line[80];

// 	//eat any leading spaces
// 	while (line[n]==' ') {
// 		n++;
// 	}

// 	//ignore lines starting with "#" (comments)
// 	if (line[n] == '#') {
// 		return;
// 	}

// 	//static char PROP_NAME_BUFFER[80];
// 		//build string "<prop name>=%d" into TEMP_BUFFER
// 	sprintf(prop_line, "%s=%%d", prop_name, *prop_value_ptr);

// 	//now, capture the prop name
// 	if (sscanf(prop_line,prop_name, &prop_value) != 0) {
// 		if (process_prefs_entry(p, prop_name, prop_value) {
// 			return true;
// 		} else {
// 			printf("Error processing line:%s\n", line);
// 			return false;
// 		}			
// 	}
// 	else {
// 		printf("Malformed processed property line %s\n", prop_line);
// 		return false;
// 	}

// 	return true;
// }


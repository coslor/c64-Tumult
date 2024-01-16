#pragma once

 #ifndef PREFS_C
 #define PREFS_C 1

#include "includes/prefs.h"
#include "includes/file_io.h"
#include <c64/keyboard.h>
#include <c64/types.h>

#define MAX_BUFFER_LEN 			80
#define MAX_PREF_NAME_LEN		40
#define MAX_PREF_ENTRY_COUNT	16

struct Preferences {
	Prefs_Entry			prefs_entries[MAX_PREF_ENTRY_COUNT];
};

struct Prefs_Entry {
	char 		name[MAX_PREF_NAME_LEN];
	(void *)	value;
};

struct Prefs_Entry[MAX_PREF_ENTRY_COUNT];
int num_pref_entries;




// #endif

/**********************prefs*********************************************************************/
/* open prefs file, reads the data in, setting preferences as necessary, and closes the file */
bool read_prefs(void *prefs, Prefs_Entry[] entries, byte dev_num, byte file_num, char *filename) {
	static const char SCANF_BUFFER[80];
	static const char FILE_BUFFER[80];

	if (prefs.prefs_device == 0) {
		printf("device number should not be 0\n");
		return false;
	};

	krnio_setnam(filename);
	if (krnio_open(file_num,dev_num,0)) {
		bool keep_reading = true;
		while (keep_reading) {
			int line_len=0;
			//First, skip any blank lines, or lines starting with "#"
			//NOTE: krnio_gets() calls chkin() before and clrchn() after the read
			do {
				line_len = krnio_gets(PREFS_FILE_NUM,FILE_BUFFER,max_prefs_buffer_len);
			} while ((line_len==0) || (FILE_BUFFER[0]=='#'));
make clean
			sprintf(SCANF_BUFFER, )
	printf("%s=%d\n", PREFS_INPUT_DEVICE_NUM,	prefs.input_device);
	printf("%s=%d\n", PREFS_PREFS_DEVICE_NUM,	prefs.prefs_device);
	printf("%s=%d\n", PREFS_UP_KEY, 			prefs.up_key);
	printf("%s=%d\n", PREFS_DOWN_KEY, 			prefs.down_key);
	printf("%s=%d\n", PREFS_LEFT_KEY,  			prefs.left_key);
	printf("%s=%d\n", PREFS_RIGHT_KEY,			prefs.right_key);
	printf("%s=%d\n", PREFS_FIRE_KEY, 			prefs.fire_key);




			if (line_len == 0) {
				printf("read_prefs() finished with EOL\n");
				keep_reading = false;
			} else {
				printf("ERROR read_prefs: error %d reading lines\n",line_len);
				keep_reading = false;
			}

		}
		byte status = krnio_pstatus[file_num];
		close_file(PREFS_FILE_NUM);

		if (status & 0b10111111) {			//are any bits set in the status word except for EOF?
			print_kernalio_message(file_num, "We're out of data, but EOF isn't set?!?");
			return false;
		}
		else {
			printf("Prefs file read successfully!\n");
			return true;
		}
	//}
		// } else {
		// 	//sprintf(kernalio_buffer, "Unable to process line for file %s\n", PREFS_FILE_NAME);
		// 	print_kernalio_message(1, "Unable to process line in file %s\n", PREFS_FILE_NAME);

		// 	close_file(PREFS_FILE_NUM);
		// 	return false;
		// }

	} else {
		//sprintf(kernalio_buffer, "Unable to open file %s\n", PREFS_FILE_NAME);
		print_kernalio_message(dev_num, "Unable to open file %s\n", filename);
		close_prefs_file(file_num);
		return false;
	}
}


/**
 * @brief Close the given file number, and print a warning if there's an error associated with it.
 * 
 * @param file_num 
 */
void close_prefs_file(byte file_num) {
	krnio_close(file_num);
	if (krnio_pstatus[file_num] & 0b10111111) {	//is the status anything but EOF?
			// sprintf(kernalio_buffer, "File read finished, but there's a nonzero status");
			print_kernalio_message(file_num,"File read finished, but there's a nonzero status");
	}
		
};

/**
 * @brief Read a prefs line
 * 
 * @param line 
 * @param prop_name 
 */
bool process_prefs_line(Tumult_Prefs p,char *line, int line_len) {
	byte n=0;
	byte pn_len=0;

	byte pv_len=0;

	int prop_value;
	//char *prop_name;

	static char prop_name[80];
	static char prop_line[80];

	//eat any leading spaces
	while (line[n]==' ') {
		n++;
	}

	//ignore lines starting with "#" (comments)
	if (line[n] == '#') {
		return;
	}

	//static char PROP_NAME_BUFFER[80];
		//build string "<prop name>=%d" into TEMP_BUFFER
	sprintf(prop_line, "%s=%%d", prop_name, *prop_value_ptr);

	//now, capture the prop name
	if (sscanf(prop_line,prop_name, &prop_value) != 0) {
		if (process_prefs_entry(p, prop_name, prop_value) {
			return true;
		} else {
			printf("Error processing line:%s\n", line);
			return false;
		}			
	}
	else {
		printf("Malformed processed property line %s\n", prop_line);
		return false;
	}

	return true;
}

bool *find_prop_value(char *line,char *prop_name, int *prop_value_ptr) {


}
/* 
 * prop_value should be cast to whatever the value should be, e.g. char* or int or whatever
 * 		yes, I know this is fairly crummy, but hey, it's c, whaddayou want?
 */
static bool process_prefs_entry(struct Tumult_Prefs p, char *prop_name, char *prop_value) { //char *prop_value, byte pv_len) {
		s//tatic char TEMP_BUFFER[80];


	if (strcmp(PREFS_INPUT_DEVICE_NUM, prop_name) == 0) {
		p.input_device = (byte)prop_value; //(byte)atoi(prop_value);
		printf("set prefs.input_device=%d\n", p.input_device);
		return true;
	};
	if (strcmp(PREFS_PREFS_DEVICE_NUM, prop_name) == 0) {
		p.prefs_device = (byte)prop_value; //(byte)atoi(prop_value);
		printf("set prefs.prefs_device=%d\n", p.prefs_device);
		return true;
	};
	if (strcmp(PREFS_UP_KEY,prop_name) == 0) {
		p.up_key = (byte)prop_value; //(byte)atoi(prop_value);
		printf("set prefs.up_key=%d\n", p.up_key);
		return true;
	};
	if (strcmp(PREFS_DOWN_KEY, prop_name) == 0) {
		p.down_key = (byte) prop_value; //(byte)atoi(prop_value);
		printf("set prefs.down_key =%d\n", p.down_key);
		return true;
	};
	if (strcmp(PREFS_LEFT_KEY, prop_name) == 0) {
		p.left_key = (byte)prop_value; //(byte)atoi(prop_value);
		printf("set prefs.left_key=%d\n", p.left_key);
		return true;
	};
	if (strcmp(PREFS_RIGHT_KEY, prop_name) == 0) {
		p.right_key = (byte)prop_value; //(byte)atoi(prop_value);
		printf("set prefs.%s=%d\n", p.right_key);
		return true;
	};
	
	printf("WARNING: unable to process property name:%s value:%s\n", prop_name, prop_value);
	return false;
}

bool write_prefs(char *filename) {
}

void print_prefs(Preferences prefs) {
	printf("%s=%d\n", PREFS_INPUT_DEVICE_NUM,	prefs.input_device);
	printf("%s=%d\n", PREFS_PREFS_DEVICE_NUM,	prefs.prefs_device);
	printf("%s=%d\n", PREFS_UP_KEY, 			prefs.up_key);
	printf("%s=%d\n", PREFS_DOWN_KEY, 			prefs.down_key);
	printf("%s=%d\n", PREFS_LEFT_KEY,  			prefs.left_key);
	printf("%s=%d\n", PREFS_RIGHT_KEY,			prefs.right_key);
	printf("%s=%d\n", PREFS_FIRE_KEY, 			prefs.fire_key);

}
#endif
#include "includes/new_prefs.h"
#include "includes/file_io.h"
#include <stdio.h>
#include <conio.h>
#include <stddef.h>
#include <stdbool.h>
#include <c64/types.h>
#include <c64/kernalio.h>
//extern void print_kernalio_message(byte device_num, const char *fmt, ...);

bool save_prefs(New_Prefs *prefs, byte file_num, byte channel_num, const char *filename) {
	bool success;
	//char filename_buffer[80];

	int dev_num=prefs->prefs_device;
	if (dev_num == 0) {
		printf("load_prefs:prefs->prefs->device may not be 0\n");
		return false;
	}

	//sprintf(filename_buffer, "%s,w,s",filename);

	printf("save_prefs:setting name to %s...\n", filename);
	krnio_setnam(filename);

	printf("save_prefs:opening file %d,%d,%d...\n", file_num, dev_num, channel_num);
	if (success = krnio_open(file_num, dev_num,channel_num)) {
		int	start=(int)prefs;
		int size = sizeof(New_Prefs);

		printf("save_prefs:writing %d bytes...\n", size);
		int bytes_written = krnio_write(file_num, (char *)start, size);
		if (bytes_written == size) {
			success=true;
		} else {
			print_kernalio_message(dev_num,"save_prefs: tried to write %d bytes,\nbut only wrote %d bytes\n", 
				size, bytes_written);

			success=false;
		}

		krnio_close(file_num);
		krnio_clrchn();
		printf("save_prefs:file closed!\n");

		if (!success) {
			print_kernalio_message(dev_num,"save_prefs:error printing preferences");
		}
	} else {
		print_kernalio_message(dev_num, "save_prefs:error opening file %d,%d,0,\"%s\"",file_num, dev_num, filename);
	}
	return success;
}

bool load_prefs(New_Prefs *prefs, byte file_num, byte channel_num, const char *filename) {
	bool success;
	//char filename_buffer[80];
	int dev_num=prefs->prefs_device;
	if (dev_num == 0) {
		printf("load_prefs:prefs->prefs->device may not be 0\n");
		return false;
	}

	krnio_setnam("");
	if (!krnio_open(15,dev_num,15)) {
		print_kernalio_message(dev_num,"Unable to open command channel");
		return false;
	}

	//sprintf(filename_buffer, "%s,r,s", filename);
	printf("load_prefs:setting name \"%s\"...\n",filename);
	krnio_setnam(filename);

	printf("load_prefs:opening file %d,%d,%d...\n", file_num, dev_num, channel_num);
	if (success = krnio_open(file_num, dev_num,channel_num)) {
		printf("load_prefs:reading preferences...\n");
		int bytes_read = krnio_read(file_num, (byte *)prefs, sizeof(New_Prefs));
		if (bytes_read == sizeof(New_Prefs)) {
			printf("load_prefs:read complete!\n");
			success=true;
		} else {
			success = false;
			print_kernalio_message(dev_num,"load_prefs:tried to read %d bytes,\nbut only got %d bytes\n", 
				sizeof(New_Prefs), bytes_read);

			success=false;
		}
	} else {
		print_kernalio_message(dev_num, "load_prefs:unable to open file %d,%d,%d,'%s'", 
			file_num, dev_num, channel_num, filename);

		char scan_buffer[80];
		scan_buffer[0]=0;
		if ( krnio_chkin(15)) {
			scanf("%19[^\n]s", scan_buffer);
			for (int i=0;i<strlen(scan_buffer); i++) {
				if (scan_buffer[i]==10) { //CTRL-J eke linefeed
					scan_buffer[i]=0;
				}
			}
			printf("drive status:%s\n");

			//scanf("%s\n",scan_buffer);
			//printf("Read from #15:\"%s\"\n",scan_buffer);
			krnio_clrchn();
			// int err,track,sector;
			// err=track=sector=-1;
			// scanf("%d,%d,%s,%d", &err,scan_buffer,&track,&sector);
			//printf("Read from #15:error code %d:\"%s\" track=%d sec=%d\n", err,scan_buffer,track,sector);
		}
	}
	krnio_close(file_num);
	krnio_close(15);

	return success;
}
		
//void get_drive_message(byte drive_num, byte *code, char**msg, byte *track, byte *sec) {

void print_prefs(New_Prefs *prefs) {
	printf("Preferences at %u:\n", 	prefs);
	printf("\tinput device:%d\n", 	prefs->input_device);
	printf("\tprefs device:%d\n", 	prefs->prefs_device);
	printf("\tup key:%d\n", 		prefs->up_key);
	printf("\tleft key:%d\n", 		prefs->left_key);
	printf("\tdown key:%d\n", 		prefs->down_key);
	printf("\tright key:%d\n", 		prefs->right_key);
	printf("\tfire key:%d\n", 		prefs->fire_key);
	printf("\tmenu key:%d\n", 		prefs->menu_key);
	printf("\n");
}

void clear_prefs(New_Prefs *prefs) {
	for (int i=0;i<sizeof(New_Prefs); i++) {
		*(((byte *)prefs)+i)=0;
	}
}

void copy_prefs(New_Prefs *dest, New_Prefs *source) {
	memcpy(dest, source, sizeof(New_Prefs));
}

void init_prefs(New_Prefs *dest_prefs, 	New_Prefs new_prefs){
	New_Prefs empty_prefs = (New_Prefs){
		.input_device=0, 
		.prefs_device=0,
		.up_key=0, 
		.left_key=0, 
		.right_key=0, 
		.down_key=0,
		.fire_key=0, 
		.menu_key=0};

	copy_prefs(dest_prefs, &new_prefs);	
	// prefs->input_device	= new_prefs.input_device;
	// prefs->prefs_device = new_prefs.prefs_device;
	// prefs->up_key		= new_prefs.up_key;
	// prefs->left_key		= left_key;
	// prefs->right_key	= right_key;
	// prefs->down_key		= down_key;
	// prefs->fire_key		= fire_key;
	// prefs->menu_key		= menu_key; 
}

void print_dir(int dev_num) {
	bool char_mode = false;
	byte c;
	int n=0;

	krnio_setnam("$");
	if (krnio_open(1,dev_num,0)) {
		while(true) {
			if (krnio_read(1,&c,1) == 1) {
				n++;
				if (c == '\0') {
				//	if (char_mode) {
						putchar(13); //newline
				//	}
					char_mode = ! char_mode;
				} 
				//else if (char_mode) {
					putchar(c);
			} else {
				if (krnio_status() & 0b10111111) {
					print_kernalio_message(dev_num,"print_dir:error reading $");
					return;
				}
				break;
			}//else krnio_read
		}//while
	} else {
	 	print_kernalio_message(dev_num,"print_dir:unable to open file \"$\" on device %d\n", dev_num);
	}
}

bool open_error_channel(int file_num, int device_num) {
	krnio_setnam(NULL);
	return krnio_open(file_num,device_num,15);
}

void close_error_channel(int file_num) {
	krnio_close(file_num);
}

int check_for_drive_error(int file_num, char *buffer, int max_buffer_len) {
	int error_num;

	buffer[0]='\0';	//zero-terminate the buffer string just in case

	
	if ((krnio_pstatus[file_num] & 0b10111111) !=0) {
		printf("kernal error status: %d\n", krnio_pstatus[file_num]);
		return -4;				//bad kernal status
	}

	if (krnio_chkin(file_num)) {
		int bytes_read = krnio_gets(file_num,buffer,max_buffer_len);
		printf("drive msg:\"%s\" bytes_read=%d\n", buffer, bytes_read);

		if (bytes_read > 0) {
			sscanf(buffer,"%d,",&error_num);
			return error_num;
		} else {
			printf("can't read line in check_for_drive_error(): bytes_read=%d\n", bytes_read);
			return -3;			//error: can't read line
		}
	} else {
		printf("can't set channel for reading in check_drive_error()\n");
		return -1;				//error:can't read from file
	}

}
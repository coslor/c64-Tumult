#include "includes/new_prefs.h"
#include "includes/file_io.h"
#include <stdio.h>
#include <conio.h>
#include <stddef.h>
#include <stdbool.h>
#include <c64/types.h>
#include <c64/kernalio.h>
//extern void print_kernalio_message(byte device_num, const char *fmt, ...);
#include "includes/logger.h"

bool save_prefs(New_Prefs *prefs, byte file_num, byte channel_num, const char *filename) {
	bool success;
	//char filename_buffer[80];

	int dev_num=prefs->prefs_device;
	if (dev_num == 0) {
		log_error("save_prefs:prefs->prefs->device may not be 0\n");
		return false;
	}

	//sprintf(filename_buffer, "%s,w,s",filename);

	log_debug("save_prefs:setting name to %s...\n", filename);
	krnio_setnam(filename);

	log_debug("save_prefs:opening file %d,%d,%d...\n", file_num, dev_num, channel_num);
	if (success = krnio_open(file_num, dev_num,channel_num)) {
		int	start=(int)prefs;
		int size = sizeof(New_Prefs);

		log_debug("save_prefs:writing %d bytes...\n", size);
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
		log_debug("save_prefs:file closed!\n");

		if (!success) {
			print_kernalio_message(dev_num,"save_prefs:error printing preferences");
		}
	} else {
		print_kernalio_message(dev_num, "save_prefs:error opening file %d,%d,0,\"%s\"",file_num, dev_num, filename);
	}
	return success;
}

//FIXME load_prefs() shows spurious errors after loading
bool load_prefs(New_Prefs *prefs, byte file_num, byte channel_num, const char *filename) {
	bool success = false;
	//char filename_buffer[80];
	int dev_num=prefs->prefs_device;
	if (dev_num == 0) {
		log_error("load_prefs:prefs->prefs->device may not be 0\n");
		return false;
	}

	krnio_setnam("");
	if (!krnio_open(15,dev_num,15)) {
		print_kernalio_message(dev_num,"Unable to open command channel");
		return false;
	}

	//sprintf(filename_buffer, "%s,r,s", filename);
	log_debug("load_prefs:setting name \"%s\"...\n",filename);
	krnio_setnam(filename);

	log_debug("load_prefs:opening file %d,%d,%d...\n", file_num, dev_num, channel_num);
	byte b= krnio_open(file_num, dev_num,channel_num);
	log_debug("b=%d\n", b);
	if (b) {
		log_debug("load_prefs:reading preferences...\n");
		int bytes_read = krnio_read(file_num, (byte *)prefs, sizeof(New_Prefs));
		if (bytes_read == sizeof(New_Prefs)) {
			log_debug("load_prefs:read complete!\n");
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
			log_debug("drive status:%s\n");

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
	log_info("Preferences at %u:\n", 	prefs);
	log_info("\tinput device:%d\n", 	prefs->input_device);
	log_info("\tprefs device:%d\n", 	prefs->prefs_device);
	log_info("\tup key:%d\n", 		prefs->up_key);
	log_info("\tleft key:%d\n", 		prefs->left_key);
	log_info("\tdown key:%d\n", 		prefs->down_key);
	log_info("\tright key:%d\n", 		prefs->right_key);
	log_info("\tfire key:%d\n", 		prefs->fire_key);
	log_info("\tmenu key:%d\n", 		prefs->menu_key);
	log_info("\n");
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
}

//TODO convert print_dir() to use logging 
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
		log_warn("kernal error status: %d\n", krnio_pstatus[file_num]);
		return -4;				//bad kernal status
	}

	if (krnio_chkin(file_num)) {
		int bytes_read = krnio_gets(file_num,buffer,max_buffer_len);
		log_debug("drive msg:\"%s\" bytes_read=%d\n", buffer, bytes_read);

		if (bytes_read > 0) {
			sscanf(buffer,"%d,",&error_num);
			return error_num;
		} else {
			log_error("can't read line in check_for_drive_error(): bytes_read=%d\n", bytes_read);
			return -3;			//error: can't read line
		}
	} else {
		log_error("can't set channel for reading in check_drive_error()\n");
		return -1;				//error:can't read from file
	}

}
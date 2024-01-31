#pragma once

#include <c64/types.h>
#include <c64/kernalio.h>

#include "includes/file_io.h"

#define MAX_KERNALIO_BUFFER_LEN 88
char kernalio_buffer[MAX_KERNALIO_BUFFER_LEN];


// bool open_disk_status(int disk_dev_num, char *disk_command) {
// 	krnio_setnam(disk_command);
// 	krnio_open()
// 	if (krnio_open(DISK_STATUS_FILE_NUM,prefs.prefs_device,15)) {
// 		return true;
// 	}
// 	print_kernalio_message(disk_dev_num, "Unable to open disk status file for device %d", prefs.prefs_device);
// 	return false;
// }

// int get_disk_status(int disk_dev_num) {
// 	int chars_read = krnio_gets(PREFS_FILE_NUM,prefs_buffer,MAX_KERNALIO_BUFFER_LEN);
// 	if (chars_read < 0) {

// 	}
// }


// char *read_disk_status(int disk_dev_num) {
// 	int chars_read = krnio_gets(kernalio_buffer)
// }
// /** NOTE: this will CLOSE any open files on the given device # disk_dev_num*/
// char *read_disk_status_complete(byte disk_dev_num, char* ) {

// }


void print_kernalio_message(byte device_num, const char *fmt, ...) {
	sformat(kernalio_buffer, fmt, (int *)&fmt + 1, false);
	int d = krnio_status();

	printf("Error:\"%s\"\nError status %d on dev %d\n", kernalio_buffer, d, device_num);
	// printf("Error status from dev %d:%d\n", device_num, krnio_pstatus[device_num]);
	printf("Press return to continue\n");
	getchar();
}


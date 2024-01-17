#include "includes/new_prefs.h"

bool save_prefs(New_Prefs *prefs, byte file_num, byte dev_num, char *filename) {
	bool success;

	if (dev_num == 0) {
		printf("device_num must be set!\n");
		return false;
	}

	printf("Setting name...\n");
	krnio_setnam(filename);

	printf("opening file %d,%d,%d...\n", file_num, dev_num, 1);
	if (success = krnio_open(file_num, dev_num,1)) {
		int	start=(int)prefs;
		int size = sizeof(New_Prefs);

		printf("writing %d bytes...\n", size);
		int bytes_written = krnio_write(file_num, (char *)start, size);
		if (bytes_written == size) {
			printf("write complete!\n");
			success=true;
		} else {
			print_kernalio_message(dev_num,"error: tried to write %d bytes,\nbut only wrote %d bytes\n", size, bytes_written);
			success=false;
		}

		krnio_close(file_num);
		krnio_clrchn();
		printf("file closed!\n");

		if (!success) {
			print_kernalio_message(dev_num,"Error printing preferences");
		}
	} else {
		print_kernalio_message(dev_num, "error opening file %d,%d,0,\"%s\"",file_num, dev_num, filename);
	}
	return success;
}

bool load_prefs(New_Prefs *prefs, byte file_num, byte dev_num, char *filename) {
	bool success;

	if (dev_num == 0) {
		printf("device_num must be set!\n");
		return false;
	}

	printf("Setting name \"%s\"...\n",filename);
	krnio_setnam(filename);

	printf("opening file %d,%d,%d...\n", file_num, dev_num, 1);
	if (success = krnio_open(file_num, dev_num,1)) {
		int bytes_read = krnio_read(file_num, (byte *)prefs, sizeof(New_Prefs));
		if (bytes_read == sizeof(New_Prefs)) {
			printf("read complete!\n");
			success=true;
		} else {
			success = false;
			print_kernalio_message(dev_num,"error: tried to read %d bytes,\nbut only got %d bytes\n", sizeof(New_Prefs), bytes_read);
			success=false;
		}
	}
	krnio_close(file_num);
	printf("file successfully closed!\n");

	return success;
}
		
void print_prefs(New_Prefs *prefs) {
	printf("Preferences at %u:", 	prefs);
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
	printf("clear_prefs():sizeof(New_Prefs)=%d\n", sizeof(New_Prefs));
	for (int i=0;i<sizeof(New_Prefs); i++) {
		*(((byte *)prefs)+i)=0;
	}
}

void copy_prefs(New_Prefs *dest, New_Prefs *source) {
	memcpy(dest, source, sizeof(New_Prefs));
}

void init_prefs(New_Prefs *dest_prefs, 	New_Prefs new_prefs){
	New_Prefs empty_prefs = (New_Prefs){
		.input_device=0, .prefs_device=0,
		.up_key=0, .left_key=0, .right_key=0, .down_key=0,
		.fire_key=0, .menu_key=0};
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

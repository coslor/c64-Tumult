/******************************************************************************
 * 			Tumult! - 	A simple game in the vein of Turmoil for the
 * 						Atari 2600. 
*******************************************************************************/

#include <string.h>
#include <conio.h>
#include <math.h>
#include <oscar.h>
#include <math.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include <gfx/bitmap.h>

#include <c64/keyboard.h>
#include <c64/vic.h>
#include <c64/sprites.h>
#include <c64/memmap.h>
#include <c64/joystick.h>
#include <c64/keyboard.h>
#include <c64/kernalio.h>

#include "includes/file_io.h"
#include "includes/tumult_prefs.h"
#include "includes/logger.h"
#include "includes/Tumult.h"
#include "includes/my_petscii.h"
//#include "includes/prefs.h"
#include "includes/file_io.h"
#include "includes/new_prefs.h"

//FIXME FIXME FIXME FIXME: fix the damn memory config! everything's broken!

//	We want our Hires screen to be at:
//		8192=0x2000=Bit 2 set to 1 in $d018 when in Hires mode
//		Note that we don't have much choice here-- our choices are 0 or 8192/$2000
//
//	We don't want to use any character set right now
//
//	We want our screen to live...right where it is now, at:
//		1024 = $400 = bits 4-7 of $d018 need to be 0001 
//
//	We want to put our sprites at:
//		8192-(sprite count*64) = 8192 - 11 * 64 = 7488 = 0x1d40 = sprite bank 117
//	**BUT**The VIC2 ALWAYS sees ROM at ranges $1000-100ff and $9000-9fff
//		...in other words, (BANK_START+1024) for banks 0 and 2
//	SO, let's put the sprites at:
//		$900 = $24 * $40
//			

//REMINDER: startup section takes $800-$863

// //#pragma region( lower, 0x0a00, 0x4000, , , {code, data} )

// //#pragma section(lower,0)
// //#pragma region(lower,0x0a00,0x4400,,,{code})

#pragma section(text_sec,0)
#pragma region(text_reg, 0x4400,0x4800,,,{text_sec})

// //0x6000-(11*64)=0x6000-704=0x6000-0x2c0= 5d40
#define SPRITE_START 0x5d40

#pragma section(sprites, 0)
#pragma region(sprites, SPRITE_START, 0x6000, , , {sprites} )

#pragma section( hires_sec, 0)
#pragma region( hires_reg, 0x6000, 0x8000, , , {hires_sec} )

// // #pragma section(bottom,1)
// // #pragma region(bottom, 0x8000,0x9000,,,{heap})

// //#pragma region( main, 0x0880,0x9000, , , {data, bss, heap, stack} );
// //#pragma region( main, 0x0880,0x9000, , , {code, data, bss, heap, stack} );
// //#pragma data(data)

// Color RAM
static char *COLOR_MEM 			= (char *)0xd800;
static char *HIRES 				= (char *)0x6000;

char SPRITE_BYTES[]={
		#embed lzo "assets/sprites/Tumult - Sprites.bin"
};
//Not sure if the __export is needed, but the tutorial examples recommend it to keep the data
//	from being optimized away by the linker
const byte PLAYFIELD_IMG[] = {
	#embed lzo "assets/tumult-back-010424.map"
};
static const byte PLAYFIELD_SCR[] = {
	#embed lzo "assets/tumult-back-010424.scr"
};

/* The sprite attrib data (basically sprite color) */
// const byte SPRITE_ATTRIBS[] = {
// 	#embed spd_sprites "assets/Tumult - SpriteAttribs.bin"
// };


//#pragma data(sprites)
//TODO: why can't I just put Sprites into the segment and load it directly?
//			I'd say it was because of the LZO compression, but the same problem happens
//			when I don't use LZO.
static char *SPRITES			= (char*)SPRITE_START;

static char *HIRES_TEXT			= (char *)0x4400;
static char *TEXT				= (char *)0x4400;


/* Handle to the start of the sprites. SPRITE_BASE=(address % 16384) / 64 */
const byte SPRITE_BASE			= (((const char *)SPRITES)/64) % 0x4000;


const byte bank			= 1;
//signed char ship_row 	= 3;
const byte MAX_SHIP_ROW	= 5;

const byte SPRITE_ROW_START = 52 + 32;
const byte SPR_ROW_HEIGHT=28;

/* Starting Y coordinate of row n */
const byte spr_rowy[] = {
	SPRITE_ROW_START + SPR_ROW_HEIGHT * 0, SPRITE_ROW_START + SPR_ROW_HEIGHT * 1, SPRITE_ROW_START + SPR_ROW_HEIGHT * 2,
	SPRITE_ROW_START + SPR_ROW_HEIGHT * 3, SPRITE_ROW_START + SPR_ROW_HEIGHT * 4, SPRITE_ROW_START + SPR_ROW_HEIGHT * 5,
	SPRITE_ROW_START + SPR_ROW_HEIGHT * 6
};

/* Handles to the sprite data for each sprite. That is, the sprite handle to display for sprite n. See SPRITE_BASE. */
const byte SHIP_LEFT_SPR_DEF	=SPRITE_BASE;
const byte SHIP_RIGHT_SPR_DEF	=SPRITE_BASE + 1;


//The values for each enum are the corresponding sprite handles;
typedef enum SHIP_SPRITE_POS { 
	SHIP_LEFT	= 0x76, 
	SHIP_RIGHT	= 0x75 
} SHIP_SPRITE_POS;
SHIP_SPRITE_POS ship_position = SHIP_LEFT;


Ship ship;

//byte ship_x = 171;

#define ADD(a,b) a+b

#define NUM_ALIENS	1

Alien aliens[NUM_ALIENS];

#define NUM_BULLETS	1
Bullet bullets[NUM_BULLETS];

#define SHIP_SPR_DEF
bool keep_looping = true;

char* final_message=NULL;

New_Prefs prefs;

byte misc_buffer[80];

const byte *PREFS_FILENAME=p"prefs2";

// typedef struct {
// 	//TODO consider fixed-point math here for sub-pixel movement

// 	signed int 		x;
// 	signed int		y;

// 	//TODO consider fractional velocity
// 	signed char		x_vel;
// 	signed char		y_vel;

// 	//the physical sprite# for this mob
// 	byte 			sprite_num;
// } Mob;

void brk_dump();
void handle_brk();
bool set_prefs();

#define BRK_VECTOR	*((int *)0x316)

int main(void) {

	//BRK_VECTOR = (int)handle_brk;

	// __asm {
	// 	lda #1
	// 	ldx #2
	// 	ldy #3

	// 	brk
	// };



	iocharmap(IOCHM_PETSCII_2);	//put printf() in lowercase mode

	if (! set_prefs()) {
		log_error("***UNABLE TO LOAD PREFS FROM %s***\n", PREFS_FILENAME);
	}
	//vic_setbank(bank);
	init_play_screen();

	// Expand hires and first color map
	oscar_expand_lzo(HIRES, PLAYFIELD_IMG);
	oscar_expand_lzo(HIRES_TEXT, PLAYFIELD_SCR);
	oscar_expand_lzo(SPRITES,SPRITE_BYTES);
		
	//Sprite attribs are just fine where they are; we'll use them later.
	//oscar_expand_lzo(Sprite,SPRITE_BYTES,SPRITE_BYTES_TO_COPY);

	byte lastJoyY=255;

	spr_init(HIRES_TEXT);


	//Set sprite colors, based on color saved in the last byte of each sprite.
	for (int i=0;i<7;i++) {
		byte spr0_color = SPRITE_BYTES[63+i] & 15; // byte 64 of sprite data set to sprite color by SpritePad
		
	}
	byte spr0_color=SPRITES[63] & 15;	//byte 64 of sprite data set to sprite color by SpritePad

	vic.spr_mcolor0=15;
	vic.spr_mcolor1=2;
	vic.spr_priority=0xff;

	
	spr_set(0, true, 100, 100, 0x75, spr0_color, true, false, false);

	ship_position = SHIP_LEFT;

	do {
		keep_looping = main_loop();
	} while (keep_looping) ;

	set_text_mode();
	vic.spr_enable=0;	//turn all sprites off
	vic.color_border=VCOL_BLUE;
	kernal_clear_screen(VCOL_BLUE, VCOL_LT_BLUE);
	iocharmap(IOCHM_PETSCII_2);	//put printf() in lowercase mode

	if (final_message) {
		printf(final_message);
		return 1;
	}
	
	return 0;
}

inline void set_mob(Mob *m, byte spr_def, byte spr_num, signed int x, signed int x_vel, byte row, bool visible, byte spr_color) {
	m->spr_def	= spr_def;
	m->spr_num	= spr_num;
	m->x 		= x;
	m->x_vel 	= x_vel;
	m->row		= row;
	m->spr_color= spr_color;
}

void init_aliens() {
	set_mob(&aliens[0].mob, 4, -50, 5, 2, true);
}

void init_ship() {
	set_mob(&ship, SHIP_LEFT, 0, 171, 0, 3, true);
}

void show_mob(Mob *m) {
	//spr_set(0, true, 100, 100, 0x75, spr0_color, true, false, false);
	//spr_set(m->spr_num, true, m->x,spr_rowy[m->row],m->spr_def,(SPRITE_BYTES[63+m->spr_num] & 15),true,false,false);

}

bool print_continue_message() {
	printf("type x to exit, return to continue\n");
	char c=0;
	do {
		c = getch();
	} while (c==0);
	return (c != 'x');
}
/** Returns whether we should keep running the main loop */
bool main_loop() {
	vic_waitFrame();

	bool result = 
		move_ship();// 	&& 
		// move_bullets() 	&& 
		// move_aliens();
	show_mob(&ship.mob);
	// show_aliens();
	// show_bullets();

	return result;
}


void show_bullets() {
	for (int i=0; i < NUM_BULLETS; i++) {
		show_mob(&bullets[i].mob);
	}
}

void show_aliens() {
	for (int i=0; i < NUM_ALIENS; i++) {
		show_mob(&bullets[i].mob);
	}
}

void die()
{
	set_text_mode();

	getch();
	set_text_mode();
}

/* Read the joystick and move the ship up or down 1 row. Updates ((50 or 60) / NUM_FRAMES_BETWEEN_MOVE_SHIP) times/second.
 *	Params:
 * 		joy_num			0 or 1 for joyports 1 and 2, or 2 for keyboard 
 * 
 *	@returns whether we should keep running the main loop
 */
bool move_ship() {
	//if (prefs.entries[PREFS_INPUT_DEVICE_NUM].value >1) {
	if (prefs.input_device > 1 ) {
		return move_ship_keyboard(); 
	}
	else {
		return move_ship_joystick(); 
	}
 }

bool move_ship_keyboard() {
	static bool last_up=false, last_left=false, last_right=false, last_down=false;

	keyb_poll();
	if (key_pressed(prefs.input_device)) {
		if (! last_down) {
			ship.mob.row++;
			if (ship.mob.row > MAX_SHIP_ROW ) {
				ship.mob.row = MAX_SHIP_ROW;
			}
		}
		last_up=false;
		last_down=true;
	} else if (key_pressed(prefs.input_device)){
		if (! last_up) {
			ship.mob.row--;
			if (ship.mob.row<0) {
				ship.mob.row=0;
			}
		}
	last_up=true;
	last_down=false;
	}
	else {
		last_up=false;
		last_down=false;
	}

	//spr_move(0, ship_x, spr_rowy[ship_row]);
	//show_mob(&ship.mob);

	if (key_pressed(prefs.input_device)) {
		ship_position=SHIP_LEFT;
		last_left=true;
		last_right=false;
	} else if (key_pressed(prefs.input_device)) {
		ship_position=SHIP_RIGHT;
		last_right=true;
		last_left=false;
	} else{
		last_right=false;
		last_left=false;;
	}
	//spr_image(0,ship_position);
	ship.mob.spr_def = ship_position;

	if (key_pressed(prefs.input_device)) {
		ship.mob.spr_color++;
		//vic.spr_color[0]++;
	}

	return true;
};

bool move_ship_joystick() {
	//int joy_num=prefs.entries[PREFS_INPUT_DEVICE_NUM].value;

	static int poll_num=0;
	static char last_joyx[2] = {0xff, 0xff}, last_joyy[2]={0xff,0xff};

	//only poll the joystick every ([50 or 60]/NUM_FRAMES_BETWEEN_MOVE_SHIP) seconds
	if ((poll_num++ % NUM_FRAMES_BETWEEN_MOVE_SHIP) != 0) {
		return true;
	}

	int joy_num=prefs.input_device - 1;
	

	joy_poll(joy_num);
	//debounce the joystick y-axis NOTE: we don't seem to need this
	//if ((joyx[joy_num] != last_joyx[joy_num]) || (joyy[joy_num] != last_joyy[joy_num]) ) {
		ship.mob.row += joyy[joy_num];
		if (joyy[joy_num] < 0) {
			vic.color_back1 = 3;
		} else if (joyy[joy_num] > 0) {
			vic.color_back1 = 4;
		}

		if (ship.mob.row < 0)
		{
			ship.mob.row = 0;
		}
		else if (ship.mob.row > MAX_SHIP_ROW)
		{
			ship.mob.row = MAX_SHIP_ROW;
		};

		//spr_move(0, ship_x, spr_rowy[ship.mob.row]);

		if (joyx[joy_num] <0 ) {
			ship.mob.spr_def = SHIP_LEFT;
			vic.color_back1 = 1;
		}
		else if (joyx[joy_num] > 0) {
			ship.mob.spr_def = SHIP_RIGHT;
			vic.color_back1 = 2;
		}
		//spr_image(0,ship_position);

		last_joyx[joy_num] = joyx[joy_num];
		last_joyy[joy_num] = joyy[joy_num];
	//};
	return true;
};

void init_play_screen() {
	for (int i=0;i<8192;i++){
		HIRES[i]=0;
	}
	for (int i=0;i<1000;i++) {
		TEXT[i]=32;
		COLOR_MEM[i]=1;
	}
	//Set VIC to HIRES & set bank & addresses for HIRES & TEXT screens in the appropriate bank
	vic_setmode(VICM_HIRES, TEXT, HIRES);

	vic.color_border = VCOL_BLACK;
	vic.color_back = VCOL_BLACK;
}

/**
 * 	Set the VIC background color, than call the Kernal routine at $e544, which clears the screen and fills color memory with the char color. 
 * 		Sets the link table entries correctly. 
 * 
 *	NOTE: this routine depends on $0288 holding the high byte of current screen memory.
*/
void kernal_clear_screen(VICColors back_color, VICColors char_color) {
	//vic.color_border=back_color;
	vic.color_back=back_color;
	vic.color_border=back_color;
	*TEXT_COLOR=char_color;

	__asm {
		jsr KERNAL_CLRSCN
	}
}

void kernal_plot(byte row, byte col) {
	__asm {
		ldx row
		ldy col
		clc
		jsr 0xfff0
	}
}

bool move_aliens() {
	vic.color_border++;

	for (byte i=0;i<NUM_ALIENS;i++) {
		aliens[i] .mob.x += aliens[i].mob.x_vel;
		int new_y = spr_rowy[aliens[i].mob.row];
		spr_move(aliens[i].mob.spr_num, aliens[i].mob.x,new_y);
	}

	vic.color_border--;
	return true;
}

// void display_aliens() {
// 	for (char i=0;i<NUM_ALIENS;i++) {
// 		spr_move(aliens[i].spr_num, aliens[i].x, spr_rowy[aliens[i].row]);
// 	}
// }

void set_text_mode() {
	vic_setmode(VICM_TEXT, (char *)0x0400, (char *)0x1000);
	iocharmap(IOCHM_PETSCII_2);	//put printf() in lowercase mode

}

bool move_bullets() {
	vic.color_border++;

	for (byte i=0;i<NUM_ALIENS;i++) {
		bullets[i] .mob.x += bullets[i].mob.x_vel;
		int new_y = spr_rowy[bullets[i].mob.row];
		spr_move(bullets[i].mob.spr_num, bullets[i].mob.x,new_y);
	}

	vic.color_border--;
	return true;
}

/***********From here on is some stuff for testing, debugging, etc. **************************/
void print_header_line(const char *message) {
	static char space_buffer[80];

	int msg_len=strlen(message);
	int log_level_len=strlen(get_log_level_name())+1;
	int num_spaces=(40-(msg_len + log_level_len))/2;
	for (byte i=0;i<num_spaces;i++) {space_buffer[i]=' ';}
	space_buffer[num_spaces]='\0';
	log_info("%c%s%s%s\n%c",18,&space_buffer[0], &message[0], &space_buffer[0],146); //18=RVS_ON, 146=RVS OFF
}

inline void press_return() {
	char empty[1];

	if (get_log_level > INFO_LEVEL) {
		printf("press return to continue\n");
		gets(empty);
	}
}

// void display_chars(char *str) {
// 	printf("String '%s'\n\t",str);
// 	int n=0;
// 	char c;

// 	do {
// 		c=str[n++];
// 		printf("%d,",c);
// 		//if ((n++ % 8)==0) {
// 		//	printf("\n\t");
// 		//}
// 	} while ((c != '\0') && (n < 80));
// 	printf("\n");
// }

void  validate_filenames(char filenames[][], byte num_names, int error_file_num) {
	const int MAX_BUF_LEN=80;
	static char buffer[MAX_BUF_LEN];
	byte full_filename[40];


	log_debug("**validating filenames***\n");

	//bool success = true;
	char char_read;
	int st=-1;

	for (int i=0;i<num_names;i++) {
		// sprintf(full_filename, "%s,s,r", filenames[i]);
		// krnio_setnam(full_filename);
		log_debug("validating file #%d:\"%s\"\n", i, filenames[i]);
		krnio_setnam(filenames[i]);

		if (krnio_open(1,8,0)) {
			if (check_for_drive_error(error_file_num,buffer,MAX_BUF_LEN) == 0) {
				char c=0;
				if ((c=krnio_getch(1)) == 'x') {
					//printf("File opened OK: \"%s\"\n", filenames[i]);
					//success = true;
				} else {
					log_error("Error reading from file %s : st=%d c='%c'\n", filenames[i], st,c);
					//success=false;
				}//if c=kenio_getch)
			} else {
				log_error("drive error:%s", buffer);
				//success = false;
			}//if check_for_drive_error

			krnio_close(1);
		} else { 
			log_error("Unable to open file %s : st=%d", filenames[i]);
			//success = false;
		} //if krnio_open
	}
}

void create_filenames(char filenames[][], byte num_names, int error_file_num) {
	//bool success;
	int st=-1;
	const int MAX_BUF_LEN=80;
	static char buffer[MAX_BUF_LEN];
	int err_num;

	//byte full_filename[40];

	log_debug("***creating filenames***\n");

	for (int i=0;i<num_names;i++) {
		log_debug("creating #%d:\"%s\"\n", i,filenames[i]);

		krnio_setnam(filenames[i]);

		if (krnio_open((i+1),8,1)) {	//1=success
			if (err_num = check_for_drive_error(error_file_num,buffer,MAX_BUF_LEN) != 0) {
			// if (err_num = check_for_drive_error(error_file_num,buffer,MAX_BUF_LEN) != 0) {
				log_error("drive error: %s \n", buffer);
				//press_return();
				break;
			} else {
				byte chars_written = krnio_putch(1,'x');
				st=krnio_status();
				if ((chars_written != 0) || st) {
						printf("error writing to file \"%s\" : st=%d cr=%d\n",filenames[i],st, chars_written);
						press_return();
				} else {
					log_debug("file was created OK: \"%s\"\n", filenames[i]);
				}//if chars_written
			} //check_for_drive_error				
		} else { 	
			log_error("error opening file %s : st=%d\n", filenames[i], krnio_status());
			press_return();
		}//if krnio_open

		krnio_close(i+1);
	}//for
}

void run_file_tests() {
		char filenames[][]= {
		"My_File1",
		p"My_File1",
		"MY_FILE1",
		p"MY_FILE1"
	};
	if (! open_error_channel(15,8)) {
		log_error("Unable to open channel to drive %d\n", 8);
		return -1;
	}

	//if (create_filenames(filenames,4,15)) {
	create_filenames(filenames,4,15);
	print_dir(8);
	press_return();

	validate_filenames(filenames,4, 15);
	log_info("all files checked out\n");
	print_dir(8);
	press_return();
	iocharmap(IOCHM_PETSCII_2);	//put printf() in lowercase mode

	char bad_filenames[][] = { "gibberish"};
	log_info("This should fail!\n");
	validate_filenames(bad_filenames, 1, 15);
}

bool set_prefs() {
	log_info("Initializing prefs\n");
	init_tumult_prefs();
	//print_kernalio_message(8,1,"#1=%d #2=%f #3=%s",1,2.0,"hello");

	//printf("last device=%d in prefs=%d\n", LAST_DEVICE_NUM, prefs.entries[PREFS_PREFS_DEVICE_NUM].value);
	log_info("last prefs device=%d\n", prefs.prefs_device);


	log_info("Prefs at startup\n");	//RVS ON, RVS OFF
	print_prefs(&prefs);
	putch(13); //newline

	if (! print_continue_message()) {
		log_info("Exiting...\n");
		return false;
	}
	
	putchar(147); //CLR

	log_info("Saving prefs\n");

	sprintf(misc_buffer, "@0:%s",PREFS_FILENAME);

	if (!save_prefs(&prefs, 1, 1,misc_buffer)) {
		print_kernalio_message(1,"Error saving preferences!");
		return false;
	}

	log_info("Showing dir\n");
	print_dir(prefs.prefs_device);

	if (!print_continue_message() ) {
		log_info("Exiting...\n");
		return false;
	}

	clear_prefs(&prefs);

	//print_header_line("Showing empty prefs");
	//print_prefs(&prefs);
	//putch(13);	//newline

	prefs.prefs_device = LAST_DEVICE_NUM;

	log_info("Showing prefs after read\n");
	if (load_prefs(&prefs, 1,0,PREFS_FILENAME)) {
		print_prefs(&prefs);
		putch(13);
	} else {
		int st=krnio_status();
		print_kernalio_message(1,"Error code %d reading prefs from \"%s\"", st, PREFS_FILENAME);
		return false;
	}

	close_error_channel(15);
	
	return true;	
}

void handle_brk() {
	byte acc,xr,yr,sp,st;
	int ret;

	__asm{
		tsx
		//ldx #0
		lda 0x0100,x
		sta ret+1
		dex
		lda 0x101,x
		sta ret
		dex
		lda 0x101,x
		sta st
		dex
		lda 0x100,x
		sta acc
		dex
		lda 0x100,x
		sta xr
		dex
		lda 0x100,x
		sta yr
	}

	set_text_mode();
	log_error("BRK caught:A=%d X=%d Y=%d\n",acc,xr,yr);

	vic.intr_ctrl <<= 1;	
	__asm{
		rti
	}
	//exit(-1);
}

void brk_dump() {
	printf("BRK caught:A=%d X=%d Y=%d\n",*((byte *)0xfd),*((byte *)0xfe),*((byte *)0xff));
}
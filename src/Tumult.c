/******************************************************************************
 * 			Tumult! - 	A simple game in the vein of Turmoil for the
 * 						Atari 2600. 
*******************************************************************************/

/* DONE/TODO
DONE: define simple sprites for ship, shots & a few enemies
TODO: refine sprites

DONE: draw simple test-based playfield
DONE: draw cool hires playfield

DONE: make ship y movement work
TODO: make ship x movement work

TODO: make ship facing work
STARTED: make the aliens move
TODO: shooting

TODO: scoring
TODO: simple text-based score display
TODO: use (simple?) sprite multiplexing for > 8 sprites onscreen
TODO: (depends on multiplexing) upgrade score display using sprites

TODO: define pickups
TODO: design pickup sprites
TODO: implement pickups
*/
//

#include "includes/Tumult.h"
#include "includes/my_petscii.h"
#include <c64/keyboard.h>
#include <stdarg.h>
#include "includes/prefs.h"
#include "includes/file_io.h"
#include "includes/new_prefs.h"

/*
 *	MEMORY REGIONS
 *
 * 	TODO: fix gaps in the memory map
 */
//#pragma region( lower, 0x0a00, 0x2000, , , {code, data} )

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

#pragma region( lower, 0x0a00, 0x4000, , , {code, data} )

#pragma section(text_sec,0)
#pragma region(text_reg, 0x4400,0x4800,,,{text_sec})

//0x6000-(11*64)=0x6000-704=0x6000-0x2c0= 
#define SPRITE_START 0x5d40

#pragma section(sprites, 0)
#pragma region(sprites, SPRITE_START, 0x6000, , , {sprites} )

#pragma section( hires_sec, 0)
#pragma region( hires_reg, 0x6000, 0x8000, , , {hires_sec} )

#pragma region( main, 0x8001, 0xa000, , , {code, data, bss, heap, stack} );
#pragma data(data)

// Color RAM
static char *COLOR_MEM 			= (char *)0xd800;

//#pragma data(data)
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
const byte SPRITE_BASE			= (SPRITES/64) % 0x4000;


const byte bank			= 1;
signed char ship_row 	= 3;
const byte MAX_SHIP_ROW	=5;

const byte SPRITE_ROW_START = 52 + 32;
const byte SPR_ROW_HEIGHT=28;

/* Starting Y coordinate of row n */
const byte spr_rowy[] = {
	SPRITE_ROW_START + SPR_ROW_HEIGHT * 0, SPRITE_ROW_START + SPR_ROW_HEIGHT * 1, SPRITE_ROW_START + SPR_ROW_HEIGHT * 2,
	SPRITE_ROW_START + SPR_ROW_HEIGHT * 3, SPRITE_ROW_START + SPR_ROW_HEIGHT * 4, SPRITE_ROW_START + SPR_ROW_HEIGHT * 5,
	SPRITE_ROW_START + SPR_ROW_HEIGHT * 6
};

/* Handles to the sprite data for each sprite. That is, the sprite handle to display for sprite n. See SPRITE_BASE. */
const byte SHIP_LEFT_SPR_HDL	=SPRITE_BASE;
const byte SHIP_RIGHT_SPR_HDL	=SPRITE_BASE + 1;

byte ship_x = 171;

//The values for each enum are the corresponding sprite handles;
typedef enum SHIP_SPRITE_POS { 
	SHIP_LEFT	= 0x76, 
	SHIP_RIGHT	= 0x75 
} SHIP_SPRITE_POS;
SHIP_SPRITE_POS ship_position = SHIP_LEFT;

typedef struct Alien {
	byte spr_num;
	unsigned int x;
	byte row;
	byte vel;
} Alien;

#define ADD(a,b) a+b

#define NUM_ALIENS 1
Alien aliens[] = { {4,-50,0,2}};

bool keep_looping = true;

char* final_message=NULL;

//Preferences prefs;
New_Prefs prefs;

// void init_tumult_prefs() {

	
// }

// Tumult_Prefs prefs = {
// 	2,
// 	99,
// 	KSCAN_I,
// 	KSCAN_J,KSCAN_L,
// 	KSCAN_K,
// 	KSCAN_SPACE,
// 	KSCAN_STOP
// 	};


void print_header_line(const char *message) {
	static char space_buffer[80];

	putch(18); //RVS ON
	int msg_len=strlen(message);
	int num_spaces=(40-msg_len)/2;
	for (byte i=0;i<num_spaces;i++) {space_buffer[i]=' ';}
	space_buffer[num_spaces]='\0';
	printf("%s%s%s\n",&space_buffer[0], &message[0], &space_buffer[0]);
	putch(146);	//RVS OFF
}

int main(void) {

	iocharmap(IOCHM_PETSCII_2);	//put printf() in lowercase mode


	print_header_line("Initializing prefs");
	init_tumult_prefs();
	//print_kernalio_message(8,1,"#1=%d #2=%f #3=%s",1,2.0,"hello");

	//printf("last device=%d in prefs=%d\n", LAST_DEVICE_NUM, prefs.entries[PREFS_PREFS_DEVICE_NUM].value);
	printf("last prefs device=%d\n", prefs.prefs_device);


	print_header_line("Prefs at startup");	//RVS ON, RVS OFF
	print_prefs(&prefs);
	putch(13); //newline

	printf("type x to exit, or any other key to continue\n");
	char c=0;
	do {
		c = getch();
	} while (c==0);

	if (c=='x') {
		printf("Exiting...\n");
		return -1;
	}
	
	putchar(147); //CLR

	print_header_line("Saving prefs\n");
	if (!save_prefs(&prefs, 1, prefs.prefs_device, (char *)"@0:prefs2.bin")) {
		print_kernalio_message(1,"Error saving preferences!");
		return -1;
	}

	// New_Prefs empty_prefs = (New_Prefs){
	// 	.input_device=0,.prefs_device=0,
	// 	.up_key=0, .left_key=0, .right_key=0, .down_key=0,
	// 	.fire_key=0, .menu_key=0};
	// copy_prefs(&prefs, empty_prefs);
	// init_prefs( &prefs, 0,0,0,0,0,0,0,0);
	clear_prefs(&prefs);

	// set_pref_value(&prefs, PREFS_PREFS_DEVICE_NUM, (char *)"prefs_device", 0);
	// prefs.entry_count=0;

	print_header_line("Showing empty prefs");
	print_prefs(&prefs);
	putch(13);	//newline

	prefs.prefs_device = LAST_DEVICE_NUM;

	print_header_line("Showing prefs after read");
	if (load_prefs(&prefs, 1,prefs.prefs_device,(char *)"PREFS2.BIN")) {
		print_prefs(&prefs);
		putch(13);
	} else {
		print_kernalio_message(1,"Error reading prefs from \"%s\"", "prefs2.bin");
		return -1;
	}

	
	return 0;

	// printf("prefs:input_device=%d up_key=%d down_key=%d left_key=%d right_key=%d fire_key=%d break_key=%d\n",
	// 	prefs.input_device,
	// 	prefs.up_key,
	// 	prefs.down_key,
	// 	prefs.left_key,
	// 	prefs.right_key,
	// 	prefs.fire_key,
	// 	prefs.break_key);
	
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

/** Returns whether we should keep running the main loop */
bool main_loop() {
	vic_waitFrame();

	return (
		move_ship() 	&& 
		move_bullets() 	&& 
		move_aliens() 
	);
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
			ship_row++;
			if (ship_row > MAX_SHIP_ROW ) {
				ship_row = MAX_SHIP_ROW;
			}
		}
		last_up=false;
		last_down=true;
	} else if (key_pressed(prefs.input_device)){
		if (! last_up) {
			ship_row--;
			if (ship_row<0) {
				ship_row=0;
			}
		}
	last_up=true;
	last_down=false;
	}
	else {
		last_up=false;
		last_down=false;
	}

	spr_move(0, ship_x, spr_rowy[ship_row]);

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
	spr_image(0,ship_position);

	if (key_pressed(prefs.input_device)) {
		vic.spr_color[0]++;
	}

	return true;
};

bool move_ship_joystick() {
	//int joy_num=prefs.entries[PREFS_INPUT_DEVICE_NUM].value;
	int joy_num=prefs.input_device;

	static int poll_num=0;
	static char last_joyx[2] = {0xff, 0xff}, last_joyy[2]={0xff,0xff};

	joy_poll(joy_num);

	//only poll the joystick every ([50 or 60]/NUM_FRAMES_BETWEEN_MOVE_SHIP) seconds
	if ((poll_num++ % NUM_FRAMES_BETWEEN_MOVE_SHIP) != 0) {
		return true;
	}

	joy_poll(prefs.input_device - 1);
	//debounce the joystick y-axis NOTE: we don't seem to need this
	//if ((joyx[joy_num] != last_joyx[joy_num]) || (joyy[joy_num] != last_joyy[joy_num]) ) {
		ship_row += joyy[joy_num];
		if (ship_row < 0)
		{
			ship_row = 0;
		}
		else if (ship_row > MAX_SHIP_ROW)
		{
			ship_row = MAX_SHIP_ROW;
		};

		spr_move(0, ship_x, spr_rowy[ship_row]);

		if (joyx[joy_num] <0 ) {
			ship_position = SHIP_LEFT;
		}
		else if (joyx[joy_num] > 0) {
			ship_position = SHIP_RIGHT;
		}
		spr_image(0,ship_position);

		last_joyx[joy_num] = joyx[joy_num];
		last_joyy[joy_num] = joyy[joy_num];
	//};
	return true;
};

/* Send a debug message to the screen */
void out(const char *fmt, ...) {
//	#ifdef DEBUG
		char buff[50];
		// const char *fmt = "0123456789012345678901234567890123456789012345678901234567890123456789";
		sformat(buff, fmt, (int *)&fmt + 1, true);
//	#endif
}

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
	//vic.color_border++;

	for (byte i=0;i<NUM_ALIENS;i++) {
		aliens[i].x += aliens[i].vel;
		spr_move(aliens[i].spr_num, aliens[i].x, spr_rowy[aliens[i].row]);
	}

	//vic.color_border--;
	return true;
}

// void display_aliens() {
// 	for (char i=0;i<NUM_ALIENS;i++) {
// 		spr_move(aliens[i].spr_num, aliens[i].x, spr_rowy[aliens[i].row]);
// 	}
// }

void set_text_mode() {
	vic_setmode(VICM_TEXT, (char *)0x0400, (char *)0x1000);
}

bool move_bullets() {
	return true;
}



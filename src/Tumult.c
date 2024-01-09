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

#define DEBUG

#include "includes/Tumult.h"
#include "includes/my_petscii.h"
#include <stdbool.h>
#include <c64/memmap.h>
#include <gfx/bitmap.h>
#include <oscar.h>
#include <math.h>
#include <c64/vic.h>
#include <stdint.h>

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

#pragma region( main, 0x8000, 0xa000, , , {code, data, bss, heap, stack} );
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

__export static const byte PLAYFIELD_SCR[] = {
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
enum SHIP_SPRITE_POS { 
	SHIP_LEFT	= 0x76, 
	SHIP_RIGHT	= 0x75 
};
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

int main(void) {

	vic_setbank(bank);
	init_play_screen();

	// Expand hires and first color map
	oscar_expand_lzo(HIRES, PLAYFIELD_IMG);
	oscar_expand_lzo(HIRES_TEXT, PLAYFIELD_SCR);
	oscar_expand_lzo(SPRITES,SPRITE_BYTES);
	
	//memcpy(Sprites,SPRITE_BYTES,SPRITE_BYTES_TO_COPY);
	
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

	bool keep_looping = true;
	
	spr_set(0, true, 100, 100, 0x75, spr0_color, true, false, false);

	ship_position = SHIP_LEFT;

	while (keep_looping) {
		main_loop();
	};

	//vic.color_border++;
	return 0;
}

void main_loop() {
		vic_waitFrame();

		move_ship(0);
		move_aliens();
}


void die()
{
	set_text_mode();

	getch();
	set_text_mode();
}

/* Read the joystick and move the ship up or down 1 row. Updates ((50 or 60) / NUM_FRAMES_BETWEEN_MOVE_SHIP) times/second.
 *
 * 	joy_num			0 or 1 for joyports 1 and 2, or 2 for keyboard 

 */
void move_ship(byte joy_num) {
	if (joy_num == 2) { move_ship_keyboard(); }
	else { move_ship_joystick(joy_num); }
}

void move_ship_keyboard() {
	vic_setbank(0);
	vic_setmode(VICM_TEXT, TEXT, HIRES);
	kernal_clear_screen(VCOL_BLACK, VCOL_LT_BLUE);
	kernal_plot(0,0);
	printf("DOH! move_ship_keyboard() is not implemented!\n");
	while(true);
}

void move_ship_joystick(byte joy_num) {

	static int poll_num=0;
	static char last_joyx[2] = {0xff, 0xff}, last_joyy[2]={0xff,0xff};
	//printf("last_joyx[0]==%d last_joyy[0]=%d\n",last_joyx[0],last_joyy[0]);

	joy_poll(joy_num);

	if ((poll_num++ % NUM_FRAMES_BETWEEN_MOVE_SHIP) != 0) {
		return;
	}

	joy_poll(joy_num);
	//debounce the joystick y-axis
	if (! ((joyx[joy_num] == last_joyx[joy_num]) && (joyy[joy_num] == last_joyy[joy_num])) ) {
	//if (last_joyy[joy_num - 1]) {
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

		if (joyx[joy_num - 1] <0 ) {
			ship_position = SHIP_LEFT;
		}
		else if (joyx[joy_num - 1] > 0) {
			ship_position = SHIP_RIGHT;
		}
			spr_image(0,ship_position);

		last_joyx[joy_num] = joyx[joy_num];
		last_joyy[joy_num] = joyy[joy_num];
	};

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


void move_aliens() {
	//vic.color_border++;

	for (byte i=0;i<NUM_ALIENS;i++) {
		aliens[i].x += aliens[i].vel;
		spr_move(aliens[i].spr_num, aliens[i].x, spr_rowy[aliens[i].row]);
	}

	//vic.color_border--;
}

// void display_aliens() {
// 	for (char i=0;i<NUM_ALIENS;i++) {
// 		spr_move(aliens[i].spr_num, aliens[i].x, spr_rowy[aliens[i].row]);
// 	}
// }

void set_text_mode() {
	vic_setmode(VICM_TEXT, (char *)0x0400, (char *)0x1000);
}

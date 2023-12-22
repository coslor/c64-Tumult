/******************************************************************************
 * 			Tumult! - 	A simple game in the vein of Turmoil for the
 * 						Atari 2600. 
*******************************************************************************/
//TODO FIX SPRITE DEF STUFF!

//DONE: define simple sprites for ship, shots & a few enemies
//TODO: refine sprites

//DONE: draw simple test-based playfield
//TODO: draw cool hires playfield

//DONE: make ship y movement work
//TODO: make ship x movement work

//TODO: make ship facing work
//TODO: make the aliens move
//TODO: shooting

//TODO: scoring
//TODO: simple text-based score display
//TODO: use (simple?) sprite multiplexing for > 8 sprites onscreen
//TODO: (depends on multiplexing) upgrade score display using sprites

//TODO: define pickups
//TODO: design pickup sprites
//TODO: implement pickups

//

#define DEBUG

#include "includes/Tumult.h"
#include <stdbool.h>

const byte SPRITE_BYTES[] = {
	#embed  spd_sprites "assets/Tumult.spd"
};
const byte SPRITE_ATTRIBS[] = {
	#embed spd_sprites "assets/Tumult.spd"
};


byte ship_row = 3;
//TODO: BUG! we can't have 7 rows, you idiot! We need 1 for the ship & 1 for the bullet, leaving *FIVE* rows available. 
//TODO Rewrite all of the playfield code to have only 6 rows.
const byte MAX_SHIP_ROW=6;
const byte SPRITE_ROW_START = 80;
const byte SPR_ROW_HEIGHT=24;

const byte spr_rowy[] = {
	SPRITE_ROW_START + SPR_ROW_HEIGHT * 0, SPRITE_ROW_START + SPR_ROW_HEIGHT * 1, SPRITE_ROW_START + SPR_ROW_HEIGHT * 2,
	SPRITE_ROW_START + SPR_ROW_HEIGHT * 3, SPRITE_ROW_START + SPR_ROW_HEIGHT * 4, SPRITE_ROW_START + SPR_ROW_HEIGHT * 5,
	SPRITE_ROW_START + SPR_ROW_HEIGHT * 6
};

#define SPRITE_BASE 0x300 / 64
byte SPRITE_IMAGES[] = {SPRITE_BASE, SPRITE_BASE + 1, SPRITE_BASE + 3};

const byte shipX = 168;
const byte ship_image[] = {0,3};

byte ship_facing = 0;

typedef struct Alien {
	byte spr_num;
	byte x;
	byte row;
	byte vel;
} Alien;

#define NUM_ALIENS 1
Alien aliens[NUM_ALIENS] = { {4,-50,0,2}};

int main(void) {

	byte lastJoyY=255;


	iocharmap(IOCHM_PETSCII_2);


	init_play_screen();

	kernal_clear_screen(VCOL_BLACK, VCOL_RED);

	printf("                Tumult!\n");

	//
	//	Copy sprite data to the proper place in VIC2 memory, specifically SpriteData.
	//		In this case, we're using the default VIC2 memory setup.
	//
	memcpy(SpriteData, SPRITE_BYTES, SPRITE_BYTES_TO_COPY);

	spr_init(SCREEN_ADDR);

	for (int i=0;i<7;i++) {
		byte spr0_color = SPRITE_BYTES[63+i] & 15; // byte 64 of sprite data set to sprite color by SpritePad
	}
	byte spr0_color=SPRITE_BYTES[63] & 15;	//byte 64 of sprite data set to sprite color by SpritePad
	vic.spr_mcolor0=15;
	vic.spr_mcolor1=2;

	spr_set(0, true, 0, 0, SPRITE_IMAGES[0], SPRITE_BYTES[63] & 15, true, false, false);

	//spr_set(2, true, aliens[0].x, spr_rowy[aliens[0].row], SPRITE_IMAGES[2], SPRITE_BYTES[64 * 2 + 63] & 15, true, false, false);

	draw_playfield();

	//while(true);

	while (true) {

		joy_poll(0);
		moveSprWithJoy(0,0);
		if (joyb[0]!=0) {
			byte scrn_row = (ship_row+1)*4;
			for (byte i=0;i<40;i++) {
				*(SCREEN_ADDR+ (scrn_row * 40) + i) = 0x40; //horiz line
			}

			kernal_plot(0,0);
			out("line drawn at line %d        \n", scrn_row);
		}

		//move_aliens();
		vic_waitFrame();
	};

	return 0;
}

void moveSprWithJoy(byte sprNum, byte joyNum) {
	static byte poll_num=0;
	//static char last_joyx[2] = {0xff, 0xff}, last_joyy[2]={0xff,0xff};
	//printf("last_joyx[0]==%d last_joyy[0]=%d\n",last_joyx[0],last_joyy[0]);

	if ((poll_num++ % 5) != 0) {
		return;
	}

	joy_poll(joyNum);
	// //debounce the joystick y-axis
	// if (! ((joyx[joyNum] == last_joyx[joyNum]) && (joyy[joyNum] == last_joyy[joyNum])) ) {
		ship_row += joyy[0];
		if (ship_row < 0)
		{
			ship_row = 0;
		}
		else if (ship_row > MAX_SHIP_ROW)
		{
			ship_row = MAX_SHIP_ROW;
		}
		// out("%d %d\n", joyy[0], shipRow);
		kernal_plot(0,0);
		out("x=%d y=%d  ",shipX,spr_rowy[ship_row]);

		spr_move(0, shipX, spr_rowy[ship_row]);

		if (joyx[joyNum] <0 ) {
			ship_facing = 3;
		}
		else if (joyx[joyNum] > 0) {
			ship_facing = 0;
		}

		//TODO turning the ship R?L doesn't work yet
		//spr_image(0,ship_image[ship_facing]);

		// last_joyx[joyNum] = joyx[joyNum];
		// last_joyy[joyNum]=joyy[joyNum];
	// }
}

void readSpritePad(byte *fname)
{
	// SPDHeader5 spdHeader;

	// FILE *mFile;

	// if (!fopen_s(&mFile, fname, "rb")) {

	// 	fread(&spdHeader, sizeof(SPDHeader5), 1, mFile);
	// 	fclose(mFile);
	// }

	// // if (decoder == SFD_SPD_SPRITES)
	// // {
	// // 	mLimit = 64 * spdHeader.mNumSprites;
	// // 	return;
	// // }

	// // mLimit = 0;
}

void out(const char *fmt, ...) {
//	#ifdef DEBUG
		char buff[50];
		// const char *fmt = "0123456789012345678901234567890123456789012345678901234567890123456789";
		sformat(buff, fmt, (int *)&fmt + 1, true);
//	#endif
}

void init_play_screen() {
	vic.color_border = VCOL_BLACK;
	vic.color_back = VCOL_BLACK;

}

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

/**
 * 	Draw the playfield using PETSCII chars.
 * 		//TODO: maybe use hires graphics for this instead?
*/
void draw_playfield() {
	for (int row=4; row<24; row++) {
		*(SCREEN_ADDR + row*40) 			= 0x5d; //vert line
		*(SCREEN_ADDR + row * 40 + 39) 		= 0x5d; // vert line
	}

	//draw top & bottom horiz lines
	for (char i=1;i<39;i++) {
		*(SCREEN_ADDR + 120 + i) 			= 0x40; // horiz line
		*(SCREEN_ADDR + 24*40 + i)			= 0x40; // horiz line
	}

	//draw horiz lines in between, leaving a gap between cols 19-21
	for (char i = 1; i < 7; i++) {
		char scrn_row = (i) * 3+2;
		int row_offset = ((scrn_row+1) * 40);
		for (char j = 1; j < 18; j++)
		{
			*(SCREEN_ADDR + row_offset + j) = 0x40; // horiz line
		}
		for (char j = 21; j < 39; j++)
		{
			*(SCREEN_ADDR + row_offset + j) = 0x40; // horiz line
		}

		//draw horiz connectors for each row
		*(SCREEN_ADDR + row_offset) 		= 0x6b;		 // left connector
		*(SCREEN_ADDR + row_offset + 39) 	= 0x73; // right connector

		//kernal_plot(scrn_row,0);
		//out("row %d\n",scrn_row);
	}

	//draw corners
	*(SCREEN_ADDR + 3*40) 		= 0x70; //upper-left corner
	*(SCREEN_ADDR + 3*40 + 39) 	= 0x6e; //upper-right corner
	*(SCREEN_ADDR + 24*40)		= 0x6d; //lower-left corner
	*(SCREEN_ADDR + 24*40 + 39)	= 0x7d; //lower-right corner
}


void move_aliens() {
	for (byte i=0;i<NUM_ALIENS;i++) {
		aliens[i].x += aliens[i].vel;
		spr_move(aliens[i].spr_num, aliens[i].x, spr_rowy[aliens[i].row]);
	}
}

// void display_aliens() {
// 	for (char i=0;i<NUM_ALIENS;i++) {
// 		spr_move(aliens[i].spr_num, aliens[i].x, spr_rowy[aliens[i].row]);
// 	}
// }
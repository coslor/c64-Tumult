/******************************************************************************
 * 			Tumult! - 	A simple game in the vein of Turmoil for the
 * 						Atari 2600. 
*******************************************************************************/

/* DONE/TODO
DONE: define simple sprites for ship, shots & a few enemies
TODO: refine sprites

DONE: draw simple test-based playfield
TODO: draw cool hires playfield

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

//TODO add the lower section stuff
//#pragma region( lower, 0x0a00, 0x1d40, , , {code, data} )

/* The sprite data read from the file */
__export static const byte SPRITE_BYTES[] = {
// 	#embed spd_sprites "assets/Tumult - Sprites.bin"
// };
	0x00,0x00,0x00,0x0A,0x40,0x00,0xEA,0x50,0x00,0xFF,0xD4,0x00,0xEA,0xA0,0x00,0x0A,
	0x00,0x00,0x0A,0x01,0x00,0x0A,0x2A,0x40,0x2A,0xAB,0x90,0xAA,0xAF,0xEB,0xAA,0xAF,
	0xEB,0xAA,0xAF,0xEB,0x2A,0xAB,0xA0,0x0A,0x2A,0x80,0x0A,0x02,0x00,0x0A,0x40,0x00,
	0xEA,0x90,0x00,0xFF,0xE8,0x00,0xEA,0xA0,0x00,0x0A,0x80,0x00,0x00,0x00,0x00,0x8B
};

//REMINDER: startup section takes $800-$863

//0x6000-(11*64)=0x6000-704=0x6000-0x2c0= 
#define SPRITE_START 0x5d40

// #pragma section(lower_sec, 0)
// #pragma region(lower_reg, 0x8a0, 0x4000,,,{code, data})

// #pragma section(middle_sec, 0)
// #pragma region(middle_reg, 0x4000, 0x4400,,,{code, data})

#pragma region( lower, 0x0a00, 0x4000, , , {code, data} )

#pragma section(text_sec,0)
#pragma region(text_reg, 0x4400,0x4800,,,{text_sec})
//#pragma data(text_sec)
//__export char TextScreen[1024];
__export char *Text=(char *)0x4400;

// #pragma section(middle_sec2, 0)
// #pragma region(middle_reg2, 0x4800, SPRITE_START,,,{code, data})

#pragma section(sprite_sec, 0)
#pragma region(sprite_reg, SPRITE_START, 0x6000, , , {sprite_sec} )
//#pragma data(sprite_sec)
//__export char Sprites[11][64];
__export char *Sprites=(char *)SPRITE_START;

#pragma section( hires_sec, 0)
#pragma region( hires_reg, 0x6000, 0x8000, , , {hires_sec} )
//#pragma data(hires_sec)
//__export char HiresScreen[0x1000];
__export char *Hires = (char *)0x6000;

//#pragma data(cassette_buff_sec)
//__export static byte SpriteData[0x2c0];
//__export static byte *SpriteData[64];
//__export static char SpriteData[1];

#pragma region( main, 0x8000, 0xa000, , , {code, data, bss, heap, stack} );
#pragma data(data)

//Not sure if the __export is needed, but the tutorial examples recommend it to keep the data
//	from being optimized away by the linker
__export static const byte PLAYFIELD_IMG[] = {
	#embed lzo "assets/tumult-back-010424.map"
};

__export static const byte PLAYFIELD_SCR[] = {
	#embed lzo "assets/tumult-back-010424.scr"
};




//#pragma region( sprites, 0x4000, 0x4100, , , {sprites} )


//#pragma data(sprites)

//char * conat SpriteData = (char *)0x2000;

// Compressed bitmap image and color data
// static const char ImgHires[] = {
// 	#embed 8000 0 lzo "../Resources/blumba.mcimg"
// };

// static const char ImgScreen[] = {
// 	#embed 1000 8000 lzo "../Resources/blumba.mcimg"
// };

// static const char ImgColor[] = {
// 	#embed 1000 9000 lzo "../Resources/blumba.mcimg"
// };

/* The sprite attrib data (basically sprite color) */
// const byte SPRITE_ATTRIBS[] = {
// 	#embed spd_sprites "assets/Tumult - SpriteAttribs.bin"
// };


// static const byte PLAYFIELD_COLOR[] = {
// 	#embed lzo "assets/grid-hires-red.scr"

// };


// Color RAM
static char *ColorMem = (char *)0xd800;

//char *Hires	= (char *)0x2000;

//char *Screen	= (char *)0x0400;
char *Color		= (char *)0xd800;
//const char *SpriteData	= SPRITE_BYTES; //(char *)0x2000;
//char *Sprites 	= (char *)832; //(char *)0x1d40;

const byte bank=1;
// #define Color	((char *)0xd000)
// #define Hires	((char *)0x4000)

//Bitmap		Screen;



#define NUM_FRAMES_BETWEEN_MOVE_SHIP 5

signed char ship_row = 3;
const byte MAX_SHIP_ROW=5;

const byte SPRITE_ROW_START = 52 + 32;
//#define SPRITE_ROW_START 32
//const byte SPRITE_ROW_START = 32;
const byte SPR_ROW_HEIGHT=28;

/* Starting Y coordinate of row n */
const byte spr_rowy[] = {
	SPRITE_ROW_START + SPR_ROW_HEIGHT * 0, SPRITE_ROW_START + SPR_ROW_HEIGHT * 1, SPRITE_ROW_START + SPR_ROW_HEIGHT * 2,
	SPRITE_ROW_START + SPR_ROW_HEIGHT * 3, SPRITE_ROW_START + SPR_ROW_HEIGHT * 4, SPRITE_ROW_START + SPR_ROW_HEIGHT * 5,
	SPRITE_ROW_START + SPR_ROW_HEIGHT * 6
};

/* Handle to the start of the sprites. SPRITE_BASE=(address % 16384) / 64 */
#define SPRITE_BASE SpriteData / 64
//const byte SPRITE_BASE = SpriteData / 64;

/* Handles to the sprite data for each sprite. That is, the sprite handle to display for sprite n. See SPRITE_BASE. */
//byte SPRITE_IMAGES[] = {SPRITE_BASE, SPRITE_BASE + 1, SPRITE_BASE + 4};
char SPRITE_IMAGES[3];


const byte shipX = 168;
const byte ship_image[] = {12,15};

byte ship_facing = 0;

typedef struct Alien {
	byte spr_num;
	unsigned int x;
	byte row;
	byte vel;
} Alien;

inline int BANK_ADDR(int bank,int offset){ return bank*0x4000+offset; }

#define NUM_ALIENS 1
Alien aliens[] = { {4,-50,0,2}};

int main(void) {

	// vic.color_border = VCOL_BLACK;
	// vic.color_back = VCOL_BLACK;

	// vic_setbank(bank);
	init_play_screen();


	SPRITE_IMAGES[0] = 75; //(SPRITE_BASE / 64);
	SPRITE_IMAGES[1] = 76; //SPRITE_IMAGES[1]+1;
	// SPRITE_IMAGES[2] = (byte)(SPRITE_BASE + 4);


	// Install trampoline to keep system running with ROM paged out
	// mmap_trampoline();

	// Page in RAM
	// mmap_set(MMAP_RAM);

	// Expand hires and first color map
	oscar_expand_lzo(Hires, PLAYFIELD_IMG);
	// oscar_expand_lzo(Screen, PLAYFIELD_SCR);


	//oscar_expand_lzo(Sprites,SPRITE_BYTES);
	
	memcpy(Sprites,SPRITE_BYTES,SPRITE_BYTES_TO_COPY);
	// for (int i=0;i<64;i++) {
	// 	*((char*)(0x1d40+i))=SPRITE_BYTES[i];
	// }
	vic.color_border++;
	
	//Sprite attribs are just fine where they are; we'll use them later.
	//oscar_expand_lzo(Sprite,SPRITE_BYTES,SPRITE_BYTES_TO_COPY);

	// Turn IO space back on
	// mmap_set(MMAP_ROM);

	// Expand color attributes
	// oscar_expand_lzo(Color, ImgColor);

	// Set hires multicolor mode
//	vic_setmode(VICM_HIRES, Screen, Hires);




	//mmap_set(MMAP_ALL_ROM);

	// set_text_mode();

	//printf("Waiting forever now\n");
	//while(1);

	byte lastJoyY=255;

	// iocharmap(IOCHM_PETSCII_2);

	// init_play_screen();

	// kernal_clear_screen(VCOL_BLACK, VCOL_RED);

	// printf("                Tumult!\n");


	// //	Copy sprite data to the proper place in VIC2 memory, specifically SpriteData.
	// //		In this case, we're using the default VIC2 memory setup.
	// //
	//memcpy((byte *)SpriteData, SPRITE_BYTES, SPRITE_BYTES_TO_COPY);
	// for (int i=0;i<64*11;i++) {
	// 	SPRITE_BYTES_TO_COPY[i]=0xff;
	// }

	spr_init(Text);

	// for (int i=0;i<7;i++) {
	// 	byte spr0_color = SPRITE_BYTES[63+i] & 15; // byte 64 of sprite data set to sprite color by SpritePad
		
	// }
	byte spr0_color=SPRITE_BYTES[63] & 15;	//byte 64 of sprite data set to sprite color by SpritePad
	vic.spr_mcolor0=15;
	vic.spr_mcolor1=2;
	vic.spr_priority=0xff;

#define SPRITE0_HANDLE (Sprites-(char *)(bank*8192))/64

//	spr_set(0, true, 0, 0, SPRITE_IMAGES[0], SPRITE_BYTES[63] & 15, true, false, false);
	spr_set(0, true, 100, 100, 0x75, spr0_color, true, false, false);

	// spr_set(aliens[0].spr_num, true, aliens[0].x, spr_rowy[aliens[0].row], SPRITE_BASE+4, SPRITE_BYTES[64 * 4 + 63] & 15, true, false, false);


	//draw_playfield();

	//while(true);


	while (true) {
		vic_waitFrame();

		//vic.color_border++;


		joy_poll(0);
		moveSprWithJoy(0,0);
		if (joyb[0]!=0) {
			byte scrn_row = (ship_row+1)*4;
			for (byte i=0;i<40;i++) {
				*(SCREEN_ADDR+ (scrn_row * 40) + i) = CHAR_HORIZ_LINE; //horiz line
			}

			// kernal_plot(0,0);
			// out("line drawn at line %d        \n", scrn_row);
		}
		if (joyx[0]) {
			int jx=joyx[0];
			(*((char*)0x47f8))+=joyx[0];
			//printf("%c%d\n",0x93,*((char*)0x07f8));

			for (int n=0;n<400 && joyx[0]==jx;n++) {
				vic.color_border=*((char*)0x47f8);
				joy_poll(0);
			}

			//joy_poll(0);

		}

		move_aliens();

//while(true);

		//vic_waitFrame();
	};

	vic.color_border++;
	return 0;
}

void die()
{
	set_text_mode();

	getch();
	set_text_mode();
}

/* Read the joystick and move the ship up or down 1 row. Updates ((50 or 60) / NUM_FRAMES_BETWEEN_MOVE_SHIP) times/second,
	so this 
 */
void moveSprWithJoy(byte sprNum, byte joyNum) {
	static int poll_num=0;
	//static char last_joyx[2] = {0xff, 0xff}, last_joyy[2]={0xff,0xff};
	//printf("last_joyx[0]==%d last_joyy[0]=%d\n",last_joyx[0],last_joyy[0]);

	if ((poll_num++ % NUM_FRAMES_BETWEEN_MOVE_SHIP) != 0) {
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
		// kernal_plot(0,0);
		// out("x=%d y=%d  ",shipX,spr_rowy[ship_row]);

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
		Hires[i]=0;
	}
	for (int i=0;i<1000;i++) {
		Text[i]=32;
		ColorMem[i]=1;
	}
	//Set VIC to HIRES & set bank & addresses for HIRES & TEXT screens in the appropriate bank
	vic_setmode(VICM_HIRES, Text, Hires);

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

/**
 * 	Draw the playfield using PETSCII chars.
 * 		//TODO: maybe use hires graphics for this instead?
*/
// void draw_playfield() {
// 	//Yes, this code could be more efficient. So what, it only runs once & is basically
// 	//	instant anyway!

// 	for (int row=4; row<24; row++) {
// 		*(SCREEN_ADDR + row*40) 			= LEFT_VERT_LINE_CHAR;
// 		*(SCREEN_ADDR + row * 40 + 39) 		= RIGHT_VERT_LINE_CHAR;
// 	}

// 	//draw top & bottom horiz lines
// 	for (char i=1;i<39;i++) {
// 		*(SCREEN_ADDR + 120 + i) 			= UPPER_HORIZ_LINE_CHAR;
// 		*(SCREEN_ADDR + 24*40 + i)			= LOWER_HORIZ_LINE_CHAR;
// 	}

// 	//draw horiz lines in between, leaving a gap between cols 19-21
// 	for (char i = 1; i <= MAX_SHIP_ROW; i++) {
// 		char scrn_row = i*3;
// 		int row_offset = ((scrn_row+1) * 40);
// 		for (char j = 1; j < 18; j++)
// 		{
// 			*(SCREEN_ADDR + row_offset + j) = JUNCTION_HORIZ_LINE_CHAR;
// 		}
// 		for (char j = 21; j < 39; j++)
// 		{
// 			*(SCREEN_ADDR + row_offset + j) = JUNCTION_HORIZ_LINE_CHAR;
// 		}

// 		//draw horiz connectors for each row
// 		*(SCREEN_ADDR + row_offset) 		= RIGHT_JUNCTION_CHAR;
// 		*(SCREEN_ADDR + row_offset + 39) 	= LEFT_JUNCTION_CHAR;
// 	}

// 	//int a = CHAR_HORIZ_LINE
// 	//draw corners
// 	*(SCREEN_ADDR + 3*40) 		= UL_CORNER_CHAR;
// 	*(SCREEN_ADDR + 3*40 + 39) 	= UR_CORNER_CHAR;
// 	*(SCREEN_ADDR + 24*40)		= LL_CORNER_CHAR;
// 	*(SCREEN_ADDR + 24*40 + 39)	= LR_CORNER_CHAR;
// }


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

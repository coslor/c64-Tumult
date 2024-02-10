#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <string.h>
#include <conio.h>
#include <stdio.h>
#include <math.h>
#include <oscar.h>
#include <math.h>

#include <c64/joystick.h>
#include <c64/keyboard.h>
#include <c64/vic.h>
#include <c64/sprites.h>
#include <c64/memmap.h>

#include <gfx/bitmap.h>

#include "file_io.h"
#include "tumult_prefs.h"
#include "logger.h"


#define DEBUG

#define KERNAL_CLRSCN 			0xe544

#define NUM_SPRITES 			11
#define SPRITE_BYTES_TO_COPY	NUM_SPRITES * 64

//#define	SpriteData	0x2000
#define	SCREEN_ADDR				(char *)0x0400
#define	TEXT_COLOR				(char *)0x0286

/** Lives in stdio.c*/
//char *sformat(char *, const char *, int *, bool);

/** If DEBUG is defined, than send output to the text screen. Otherwise, swallow the output.*/
void out(const char *, ...);

void moveSprWithJoy(unsigned char sprNum, unsigned char joyNum);

void out(const char *fmt, ...);

void init_play_screen();

void kernal_clear_screen(VICColors back_color, VICColors char_color);

void moveSprWithJoy(unsigned char sprNum, unsigned char joyNum);

void readSpritePad(char *fname);

void out(const char *fmt, ...);

void kernal_plot(char row, char col);

//void draw_playfield();

//These should return true unless we need to stop the game.
bool move_aliens();
bool move_ship();
bool move_ship_keyboard();
bool move_ship_joystick();
bool move_bullets();

//stuff to process proprties files

/* opens prefs file, reads the data in, sets preferences as necessary, and closes the file */
void close_file(byte file_num);
//void print_kernalio_message(byte device_num, byte file_num, const char *fmt, ...);
void print_kernalio_message(unsigned char device_num, const char *fmt, ...);
//extern char * sformat(char * buff, const char * fmt, int * fps, bool print);


#define LAST_DEVICE_NUM *((byte *)0xba)

// struct SPDHeader5
// {
// 	unsigned char 	mID[3];
// 	unsigned char 	mVersion[1];
// 	unsigned char 	mFlags;
// 	unsigned int	mNumSprites, mNumTiles;
// 	unsigned char 	mNumSpriteAnmis, mNumTileAnims;
// 	unsigned char 	mTileWidth, mTileHeight;
// 	unsigned char 	mColors[3];
// 	unsigned char 	mSpriteOverlayDist, mTileOverlayDist;
// };


/* PETSCII chars to draw playfield */
// #define LEFT_VERT_LINE_CHAR		CHAR_VERT_LINE
// #define RIGHT_VERT_LINE_CHAR	CHAR_VERT_LINE
// #define UPPER_HORIZ_LINE_CHAR	CHAR_HORIZ_LINE
// #define LOWER_HORIZ_LINE_CHAR	CHAR_HORIZ_LINE
// #define UL_CORNER_CHAR			CHAR_UPPER_LEFT_CORNER
// #define UR_CORNER_CHAR			CHAR_UPPER_RIGHT_CORNER
// #define LL_CORNER_CHAR			CHAR_LOWER_LEFT_CORNER
// #define LR_CORNER_CHAR			CHAR_LOWER_RIGHT_CORNER
// #define LEFT_JUNCTION_CHAR		CHAR_LEFT_JUNCTION
// #define RIGHT_JUNCTION_CHAR		CHAR_RIGHT_JUNCTION
// #define JUNCTION_HORIZ_LINE_CHAR CHAR_HORIZ_LINE

#define LEFT_VERT_LINE_CHAR		CHAR_INVERSE_SPACE
#define RIGHT_VERT_LINE_CHAR	CHAR_INVERSE_SPACE

// #define UPPER_HORIZ_LINE_CHAR	CHAR_INVERSE_SPACE
// #define LOWER_HORIZ_LINE_CHAR	CHAR_INVERSE_SPACE
#define UPPER_HORIZ_LINE_CHAR	CHAR_INVERSE_SPACE
#define LOWER_HORIZ_LINE_CHAR	CHAR_INVERSE_SPACE

#define UL_CORNER_CHAR			CHAR_INVERSE_SPACE
#define UR_CORNER_CHAR			CHAR_INVERSE_SPACE
#define LL_CORNER_CHAR			CHAR_INVERSE_SPACE
#define LR_CORNER_CHAR			CHAR_INVERSE_SPACE
#define LEFT_JUNCTION			CHAR_INVERSE_SPACE
#define RIGHT_JUNCTION			CHAR_INVERSE_SPACE

//#define JUNCTION_HORIZ_LINE_CHAR CHAR_INVERSE_SPACE
#define JUNCTION_HORIZ_LINE_CHAR CHAR_HORIZ_LINE

#define LEFT_JUNCTION_CHAR		CHAR_INVERSE_SPACE
#define RIGHT_JUNCTION_CHAR		CHAR_INVERSE_SPACE
#define JUNCTION_HORIZ_LINE_CHAR CHAR_INVERSE_SPACE

#define NUM_FRAMES_BETWEEN_MOVE_SHIP 5

typedef struct Mob {
	byte		spr_def;	//the handle to the sprite def (sprite def address = vic_base + spr_def * 64)
	byte		spr_num;	//the physical sprite #
	signed int	x;			//a signed int so it can start out offscreen
	signed int	x_vel;
	signed char	row;		//needs to be a signed char
	bool		visible;
	byte		spr_color;
} Mob;

typedef struct Alien {
	Mob			mob;
} Alien;

typedef struct Bullet {
	Mob			mob;
} Bullet;

typedef struct Ship {
	Mob			mob;
} Ship;

void set_text_mode();
void die();
bool main_loop();

bool print_continue_message();

 
/* Valid banks are 0-3, addr must be >=0 and <0x4000 */
#define BANK_ADDR(bank,offset) (int)((long)(bank*0x4000)+offset)

//#pragma compile("/../Tumult.c")

void show_mob(Mob *m);
void show_bullets();
void show_aliens();

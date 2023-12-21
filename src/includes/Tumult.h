#include <c64/vic.h>
#include <c64/sprites.h>
#include <string.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
//#include <stdarg.h>
#include <c64/joystick.h>

#define KERNAL_CLRSCN 0xe544

const char 			NUM_SPRITES = 1;
const int 			SPRITE_BYTES_TO_COPY = NUM_SPRITES * 64;

#define	SpriteData	(char *)0x0380
#define	SCREEN_ADDR	(char *)0x0400
#define	TEXT_COLOR	(char *)0x0286

/** Lives in stdio.c*/
char *sformat(char *, const char *, int *, bool);

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

void draw_playfield();

void move_aliens();

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
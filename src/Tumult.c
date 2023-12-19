/******************************************************************************
 * 			Tumult! - 	A simple game in the vein of Turmoil for the
 * 						Atari 2600. 
*******************************************************************************/

#include <c64/vic.h>
#include <c64/sprites.h>
#include <string.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
//#include <stdarg.h>
#include <c64/joystick.h>

#define DEBUG

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

// struct loc {
// 	int x,y;
// };

const char SPRITE_BYTES[] = {
	#embed  spd_sprites "assets/Tumult.spd"
};

const char SPRITE_ATTRIBS[] = {
	#embed spd_sprites "assets/Tumult.spd"
};

// enum SourceFileDecoder
// {
// 	SFD_NONE,
// 	SFD_CTM_CHARS,
// 	SFD_CTM_CHAR_ATTRIB_1,
// 	SFD_CTM_CHAR_ATTRIB_2,
// 	SFD_CTM_TILES_8,
// 	SFD_CTM_TILES_16,
// 	SFD_CTM_MAP_8,
// 	SFD_CTM_MAP_16,
// 	SFD_SPD_SPRITES
// };


#define SpriteData ((char *)0x0380)
#define NUM_SPRITES 1
#define SPRITE_BYTES_TO_COPY NUM_SPRITES * 64

char *const SCREEN_ADDR = (char *)0x0400;


/** Lives in stdio.c*/
char *sformat(char *, const char *, int *, bool);

/** If DEBUG is defined, than send output to the text screen. Otherwise, swallow the output.*/
void out(const char *, ...);

void moveSprWithJoy(unsigned char sprNum, unsigned char joyNum);

sbyte shipRow = 3;
const int MAX_SHIP_ROW=6;
const char sprRowY[] = {50+0,50+28,50+56,50+84,50+112,50+140,50+168};

int main(void) {

	char lastJoyY=255;
	char shipX = 160;

	iocharmap(IOCHM_PETSCII_2);

	// #if DEBUG=1
	// 	out(("sprite data is:\n"));
	// 	for (int i=0;i<4;i++) {
	// 		printf("%x:\t",i*16);

	// 		for (int j=0;j<16;j++) {
	// 			printf(("%x ",SPRITE_BYTES[i*16+j]));
	// 		}
	// 		out(()"\n"));
	// 	}
	// #endif

	out("copying %d sprites...\n", NUM_SPRITES);
	memcpy(SpriteData, SPRITE_BYTES, SPRITE_BYTES_TO_COPY);

	out("initializing sprites...\n");
	spr_init(SCREEN_ADDR);


	char spr0_color=SPRITE_BYTES[63] & 15;
	out("sprite color=%d\n",spr0_color);
	vic.spr_mcolor0=15;
	vic.spr_mcolor1=2;

	out("showing sprite...\n");
	/** void spr_set(char sp, bool show, int xpos, int ypos, char image, char color, bool multi, bool xexpand, bool yexpand); **/
	spr_set(0,true,0,0,(unsigned)(SpriteData)/64,spr0_color,true,false,false);

	// char y = 0;
	// while (true)
	// {
	// 	for (char i = 0; i < 8; i++)
	// 		spr_move(i, 30 + 20 * i, 60 + 20 * i + y);
	// 	y++;
	// 	vic_waitFrame();
	// }
	

	out("waiting forever...\n");
	while (true) {
		joy_poll(0);
		shipRow += joyy[0];
		if (shipRow<0) {
			shipRow=0;
		}
		else if (shipRow>MAX_SHIP_ROW) {
			shipRow = MAX_SHIP_ROW;
		}
		out("%d %d\n", joyy[0], shipRow);
		spr_move(0,shipX,sprRowY[shipRow]);
		// moveSprWithJoy(0,0);
		vic_waitFrame();
	};

	return 0;
}

void moveSprWithJoy(unsigned char sprNum, unsigned char joyNum) {
	joy_poll(joyNum);
	spr_move(sprNum,spr_posx(sprNum) + joyx[joyNum], spr_posy(sprNum)+joyy[joyNum]);
}
// void out(const char *fmt, ...) {
// 	#ifdef DEBUG
// 		va_list argPtr;
// 		va_start(argPtr,fmt);
// 		vprintf(fmt, argPtr);
// 		va_end(fmt, argPtr);
// 	#endif
// }

void readSpritePad(char *fname)
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
	#ifdef DEBUG
		char buff[50];
		// const char *fmt = "0123456789012345678901234567890123456789012345678901234567890123456789";
		sformat(buff, fmt, (int *)&fmt + 1, true);
	#endif
}
/******************************************************************************
 * 			Tumult! - 	A simple game in the vein of Turmoil for the
 * 						Atari 2600. 
*******************************************************************************/


#define DEBUG

#include "includes/Tumult.h"


const char SPRITE_BYTES[] = {
	#embed  spd_sprites "assets/Tumult.spd"
};

const char SPRITE_ATTRIBS[] = {
	#embed spd_sprites "assets/Tumult.spd"
};


signed char ship_row = 3;
const char MAX_SHIP_ROW=7;
const char SPRITE_ROW_START = 72;
const char SPR_ROW_HEIGHT=24;

const char spr_rowy[] = {
	SPRITE_ROW_START + SPR_ROW_HEIGHT * 0, SPRITE_ROW_START + SPR_ROW_HEIGHT * 1, SPRITE_ROW_START + SPR_ROW_HEIGHT * 2,
	SPRITE_ROW_START + SPR_ROW_HEIGHT * 3, SPRITE_ROW_START + SPR_ROW_HEIGHT * 4, SPRITE_ROW_START + SPR_ROW_HEIGHT * 5
};

const char shipX = 180;

int main(void) {

	char lastJoyY=255;

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

	init_play_screen();

	kernel_clear_screen(VCOL_BLACK, VCOL_RED);

	out("                Tumult!\n");

	// out("copying %d sprites...\n", NUM_SPRITES);
	memcpy(SpriteData, SPRITE_BYTES, SPRITE_BYTES_TO_COPY);

	//out("initializing sprites...\n");
	spr_init(SCREEN_ADDR);


	char spr0_color=SPRITE_BYTES[63] & 15;
	//out("sprite color=%d\n",spr0_color);
	vic.spr_mcolor0=15;
	vic.spr_mcolor1=2;

	//out("showing sprite...\n");
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
	
	draw_playfield();

	while (true) {

		joy_poll(0);
		// shipRow += joyy[0];
		// if (shipRow<0) {
		// 	shipRow=0;
		// }
		// else if (shipRow>MAX_SHIP_ROW) {
		// 	shipRow = MAX_SHIP_ROW;
		// }
		//out("%d %d\n", joyy[0], shipRow);
		//spr_move(0,shipX,sprRowY[shipRow]);
		moveSprWithJoy(0,0);
		if (joyb[0]!=0) {
			char scrn_row = (ship_row+1)*4;
			for (char i=0;i<40;i++) {
				*(SCREEN_ADDR+ (scrn_row * 40) + i) = 0x40; //horiz line
			}

			kernel_plot(0,0);
			out("line drawn at line %d        \n", scrn_row);
		}
		vic_waitFrame();
	};

	return 0;
}

void moveSprWithJoy(unsigned char sprNum, unsigned char joyNum) {
	static unsigned char poll_num=0;
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
		spr_move(0, shipX, spr_rowy[ship_row]);
		// last_joyx[joyNum] = joyx[joyNum];
		// last_joyy[joyNum]=joyy[joyNum];
	// }
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
//	#ifdef DEBUG
		char buff[50];
		// const char *fmt = "0123456789012345678901234567890123456789012345678901234567890123456789";
		sformat(buff, fmt, (int *)&fmt + 1, true);
//	#endif
}

void init_play_screen() {
	vic.color_border = VCOL_WHITE;
	vic.color_back = VCOL_BLACK;

}

void kernel_clear_screen(VICColors back_color, VICColors char_color) {
	//vic.color_border=back_color;
	vic.color_back=back_color;
	*TEXT_COLOR=char_color;

	__asm {
		jsr KERNEL_CLRSCN
	}
}

void kernel_plot(byte row, byte col) {
	__asm {
		ldx row
		ldy col
		clc
		jsr 0xfff0
	}
}

void draw_playfield() {
	for (char i = 0; i < 8; i++) {
		char scrn_row = (i) * 3+2;
		for (char j = 0; j < 40; j++)
		{
			*(SCREEN_ADDR + (scrn_row * 40) + j) = 0x40; // horiz line
		}
		kernel_plot(scrn_row,0);
		out("row %d\n",scrn_row);
	}

}


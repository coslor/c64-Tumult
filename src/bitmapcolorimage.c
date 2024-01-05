#include <c64/vic.h>
#include <c64/memmap.h>
#include <oscar.h>
#include <string.h>
#include "includes/bitmapcolorimage.h"

#define USE_LZO true

// Place bitmap memory underneath kernal ROM
char * const Hires	= (char *)0xe000;

// Place cell color map underneath IO
char * const Screen	= (char *)0xd000;
char * const Color	= (char *)0xd800;

// Compressed bitmap image and color data
static const char ImgHires[] = {
#ifdef USE_LZO	
	#embed 8000 0 lzo "assets/blumba.mcimg"
#else
	#embed 8000 0 "assets/blumba.mcimg"
#endif	
};

static const char ImgScreen[] = {
#ifdef USE_LZO
	#embed 1000 8000 lzo "assets/blumba.mcimg"
#else
	#embed 1000 8000 "assets/blumba.mcimg"
#endif
};

static const char ImgColor[] = {
#ifdef USE_LZO	
	#embed 1000 9000 lzo "assets/blumba.mcimg"
#else
	#embed 1000 9000 "assets/blumba.mcimg"
#endif
};

int main(void) {
	// Install trampoline to keep system running with ROM paged out
	mmap_trampoline();

	// Page in RAM
	mmap_set(MMAP_RAM);

	setPictureBytes();

	// Turn IO space back on
	mmap_set(MMAP_ROM);

	// Expand color attributes
	oscar_expand_lzo(Color, ImgColor);

	// Set hires multicolor mode
	vic_setmode(VICM_HIRES_MC, Screen, Hires);
	vic.color_border = VCOL_BLACK;
	vic.color_back = VCOL_BLACK;

	for(;;) 
		;

	return 0;
}

void setPictureBytes() {
	// Expand hires and first color map
	//TODO why does this work properly when we're using LZO but not otherwise?
#ifdef USE_LZO	
	oscar_expand_lzo(Hires, ImgHires);
	oscar_expand_lzo(Screen, ImgScreen);
#else
	memcpy(Hires, ImgHires, 8000);
	memcpy(Screen, ImgScreen, 1000);
	memcpy(Color, ImgColor, 1000);
#endif
}

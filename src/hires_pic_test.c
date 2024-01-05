#include <c64/vic.h>
#include <c64/memmap.h>
#include <oscar.h>

// Place bitmap memory underneath kernal ROM
char *const Hires = (char *)0xe000;

// Place cell color map underneath IO
char *const Screen = (char *)0xd000;
char *const Color = (char *)0xd800;

// Compressed bitmap image and color data
static const char ImgHires[] = {
#embed lzo "assets/grid-hires-dont-shoot-food-gaps.map"
};

static const char ImgScreen[] = {
#embed lzo "assets/grid-hires-dont-shoot-food-gaps.scr"
};

// static const char ImgColor[] = {
// 	#embed 1000 9000 lzo "../Resources/blumba.mcimg"
// };

int main(void)
{
	// Install trampoline to keep system running with ROM paged out
	mmap_trampoline();

	// Page in RAM
	mmap_set(MMAP_RAM);

	// Expand hires and first color map
	oscar_expand_lzo(Hires, ImgHires);
	oscar_expand_lzo(Screen, ImgScreen);

	// Turn IO space back on
	mmap_set(MMAP_ROM);

	// Expand color attributes
	// oscar_expand_lzo(Color, ImgColor);

	// Set hires multicolor mode
	vic_setmode(VICM_HIRES, Screen, Hires);
	vic.color_border = VCOL_BLACK;
	vic.color_back = VCOL_BLACK;

	for (;;)
		;

	return 0;
}

#include "FayConsoleEngine.h"

/*
	Compile

	Open cygwin terminal

	>cd /cygdrive/c/Users/Alifa/Desktop/cygwin64-workspace/mystuff/fayconsoleengine/
	>gcc gcc FayMain.c -o out.bin
	>./out.bin
*/

int main() {
	// 20x18 tiles/characters. Each tile has 8xscale pixels across and 8xscale pixels down 
	init(100, 100, 8, 8, 2);

	// define some chars
	wchar_t a = PIXEL_SOLID;
	wchar_t b = 'F';
	wchar_t c = ' ';

	// clear screen to black if needed
	fillRect(c, getPoint(0,0), getPoint(len,hei), BG_BLACK);

	// do some funky stuff
	// fillRect(b, getPoint(0,0), getPoint(5,5), FG_MAGENTA | BG_RED);

	// do more funky stuff
	// fillRect(c, getPoint(5,5), getPoint(10,10), BG_WHITE);

	// Show palette
	// init(160, 140, 8, 8, 1) for best results
	 showPalette();

	// Go nuts
	// init(100, 100, 8, 8, 2) for best results
	// while (1) {
	// 	goNuts();
	//	refresh();
	// }

	// finally draw!
	refresh();

	Sleep(10000);

	// cleanup
	dismiss();
}
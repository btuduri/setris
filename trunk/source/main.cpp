/*
Copyright (c) 2008 Daniel Wickert

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <PA9.h>
#include <stdio.h>
#include <string.h>

#include "game_loop.h"

static const unsigned short NUM_TILES = 81;

// Function: main()
int main(int argc, char ** argv)
{
	
	// PA_LoadSpritePal(0, // Screen
		// 0, // Palette number
		// (void*)stones_Pal);	// Palette name

	// size_t i;
	// for (i = 0; i < 48; ++i)
	// {
		// s16 x = (i % 8) * 32;
		// s16 y = (i / 8) * 32;
								
		// PA_CreateSprite(0, // Screen
				// i, // Sprite number
				// (void*)(stones_Sprites + i*32*32), // Sprite data
				// OBJ_SIZE_32X32, // Sprite size
				// 1, // 256 color mode
				// 0, // Sprite palette number
				// x, y); // X and Y position on the screen
	// }
		
	// // Infinite loop to keep the program running
	// while (1)
	// {
		// PA_WaitForVBL();
	// }
	
	GameLoop game;
	game.run();
	
	return 0;
}

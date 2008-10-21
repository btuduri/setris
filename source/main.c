// Includes
#include <PA9.h>       // Include for PA_Lib
#include <stdio.h>
#include <string.h>


#include "music.h"
#include "stones_data.cpp"

static const unsigned short NUM_TILES = 81;

// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();
	PA_InitVBL();
	
	PA_PlayMod(music);
	
	PA_LoadSpritePal(0, // Screen
		0, // Palette number
		(void*)stones_Pal);	// Palette name

	size_t i;
	for (i = 0; i < 41; ++i)
	{
		s16 x = (i % 8) * 32;
		s16 y = (i / 8) * 32;
								
		PA_CreateSprite(0, // Screen
				i, // Sprite number
				(void*)(stones_Sprites + i*32*32), // Sprite data
				OBJ_SIZE_32X32, // Sprite size
				1, // 256 color mode
				0, // Sprite palette number
				x, y); // X and Y position on the screen
	}
		
	// Infinite loop to keep the program running
	while (1)
	{
		PA_WaitForVBL();
	}
	
	return 0;
}

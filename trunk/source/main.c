// Includes
#include <PA9.h>       // Include for PA_Lib
#include <stdio.h>
#include <string.h>


#include "music.h"
#include "gfx/all_gfx.h"
#include "gfx/all_gfx.c"

static const unsigned short NUM_TILES = 81;
const unsigned char** TILES_DATA_PTRS;
const unsigned short** PAL_DATA_PTRS;

void init_sprite_data();

// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	init_sprite_data();
		
	PA_PlayMod(music);
	
	// Infinite loop to keep the program running
	while (1)
	{
		PA_WaitForVBL();
	}
	
	return 0;
}

#define PUT_TILE(i) \
    TILES_DATA_PTRS[i] = tiles_##i##_Sprite;\
	PAL_DATA_PTRS[i] = tiles_##i##_Pal;
	
void init_sprite_data()
{
	TILES_DATA_PTRS = (const unsigned char**) malloc(NUM_TILES * sizeof(const unsigned char*));
	PAL_DATA_PTRS = (const unsigned short**) malloc(NUM_TILES * sizeof(const unsigned short*));
	PUT_TILE(0);
	PUT_TILE(1);
	PUT_TILE(2);
	PUT_TILE(3);
	PUT_TILE(4);
	PUT_TILE(5);
	PUT_TILE(6);
	PUT_TILE(7);
	PUT_TILE(8);
	PUT_TILE(9);
	PUT_TILE(10);
	PUT_TILE(11);
	PUT_TILE(12);
	PUT_TILE(13);
	PUT_TILE(14);
	PUT_TILE(15);
	PUT_TILE(16);
	PUT_TILE(17);
	PUT_TILE(18);
	PUT_TILE(19);
	PUT_TILE(20);
	PUT_TILE(21);
	PUT_TILE(22);
	PUT_TILE(23);
	PUT_TILE(24);
	PUT_TILE(25);
	PUT_TILE(26);
	PUT_TILE(27);
	PUT_TILE(28);
	PUT_TILE(29);
	PUT_TILE(30);
	PUT_TILE(31);
	PUT_TILE(32);
	PUT_TILE(33);
	PUT_TILE(34);
	PUT_TILE(35);
	PUT_TILE(36);
	PUT_TILE(37);
	PUT_TILE(38);
	PUT_TILE(39);
	PUT_TILE(40);
	PUT_TILE(41);
	PUT_TILE(42);
	PUT_TILE(43);
	PUT_TILE(44);
	PUT_TILE(45);
	PUT_TILE(46);
	PUT_TILE(47);
	PUT_TILE(48);
	PUT_TILE(49);
	PUT_TILE(50);
	PUT_TILE(51);
	PUT_TILE(52);
	PUT_TILE(53);
	PUT_TILE(54);
	PUT_TILE(55);
	PUT_TILE(56);
	PUT_TILE(57);
	PUT_TILE(58);
	PUT_TILE(59);
	PUT_TILE(60);
	PUT_TILE(61);
	PUT_TILE(62);
	PUT_TILE(63);
	PUT_TILE(64);
	PUT_TILE(65);
	PUT_TILE(66);
	PUT_TILE(67);
	PUT_TILE(68);
	PUT_TILE(69);
	PUT_TILE(70);
	PUT_TILE(71);
	PUT_TILE(72);
	PUT_TILE(73);
	PUT_TILE(74);
	PUT_TILE(75);
	PUT_TILE(76);
	PUT_TILE(77);
	PUT_TILE(78);
	PUT_TILE(79);
	PUT_TILE(80);
}

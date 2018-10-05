#include <peekpoke.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../../graphics_settings.h"

// #include "../../../graphics_data/8x8_chars.h"

// #include "../../../redefine_characters/udg_map.h"

#define UDG_BASE_FACTOR 15


void INIT_GRAPHICS(void)
{
	unsigned short i;

	POKE(65299ul,(PEEK(65299ul)&3)|((15)*4)); // change character base address to 28th Kbyte
	POKE(65298ul,PEEK(65298ul)&251); // make graphics chip get characters from RAM
	
	
	// {
		// unsigned short i;
		// for(i=0;i<64;++i)
		// {
			// POKE((3072+i),i+0x40);
		// }
		
		// sleep(4);
	// }
	
	// while(1){};	

	for(i=0;i<1000;++i)
	{
		POKE(2048+i,0x71);
	}
			
	// do
	// {
		// POKE(2048+i,0x71);
		// POKE(2048+256+i,0x71);
		// POKE(2048+512+i,0x71);
		// POKE(2048+1000-256+i,0x71);
		// ++i;
	// } while(i<255);
	
}

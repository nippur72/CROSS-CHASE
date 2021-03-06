#include <peekpoke.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "graphics_settings.h"

#include "8x8_chars.h"

#include "udg_map.h"

#include <stdint.h>

void INIT_GRAPHICS(void)
{
	uint16_t i;
	
	POKE(1177,62); // disable switch to RAM in PEEK
	for(i=0;i<1023;++i)
	{
		POKE(UDG_BASE_FACTOR*1024+i,PEEK((uint16_t) (54272ul+(uint16_t) i)));
	}
	POKE(1177,63); // re-enable switch to RAM in PEEK
	POKE(65299ul,(PEEK(65299ul)&3)|((UDG_BASE_FACTOR)*4)); // change character base address to 28th Kbyte
	POKE(65298ul,PEEK(65298ul)&251); // make graphics chip get characters from RAM
		
	REDEFINE_AT(UDG_BASE_FACTOR*1024);
}


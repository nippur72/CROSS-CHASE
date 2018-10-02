
#include "../graphics_mode/memory_mapped_graphics.h"
#include "memory_mapped_alt_print.h"

#if defined(BUFFERED)	

	#include "../display_target_geometry.h"
	
	extern unsigned char video_buffer[YSize+Y_OFFSET][XSize];
#endif

#if defined(__CMOC__) && !defined(__WINCMOC__)
	#include <cmoc.h>
#else
	#include <stdlib.h>
	#include <string.h>
	#include <stdio.h>
#endif


#if defined(CBM_SCREEN_CODES)
	char screenCode(char ch)
	{
		if(ch>64)
		{
			return ch-64;
		}
		else
		{
			return ch;
		}
		return ch;
	}
#elif defined(__CMOC__) && !defined(__WINCMOC__)
	char screenCode(char ch)
	{
		if(ch==32) 
		{
			return 32+64;
		}
		else
		{
			return ch-32;
		}	
	}
#elif defined(__C16__) && defined(C16_UNEXPANDED)
	char screenCode(char ch)
	{
		if(ch<64)
		{
			return 64+ch;
		}
		else
		{
			return ch;
		}	
	}
#endif

#  if defined(Z88DK_SPRITES)
	#include <graphics.h>
	#include <games.h>

	#define _DISPLAY(x,y,ch) \
		_draw_ch(x,y,ch);
	
#elif defined(BUFFERED)
	#define _DISPLAY(x,y,ch) \
		video_buffer[(y)][(x)] = (ch)
		
#elif defined(__C16__) && defined(C16_UNEXPANDED)
	#define _DISPLAY(x,y,ch) \
		do \
		{ \
			DISPLAY_POKE((loc(x,y)), (ch)); \
			DISPLAY_POKE((loc(x,y)-1024), 0x71); \
		} while(0)
#else
	#define _DISPLAY(x,y,ch) \
		DISPLAY_POKE((loc(x,y)), (ch))
#endif


void PRINT(unsigned char x, unsigned char y, char * str)
{
	unsigned char i = 0;
	while(str[i]!='\0')
	{
		#if defined(CBM_SCREEN_CODES) || (defined(__CMOC__) && !defined(__WINCMOC__)) || (defined(__C16__) && defined(C16_UNEXPANDED))
			_DISPLAY(x+i,y, screenCode(str[i]));
		#else
			_DISPLAY(x+i,y, str[i]);
		#endif
		++i;
	}
}

void print_05u0(unsigned char x, unsigned char y, unsigned short val)
{
	unsigned char i;
	unsigned char digits[6];
	
	digits[0] = 0;
	for(i=1;i<6;++i)
	{
		digits[i] = (unsigned char) ((val)%10);
		val-= digits[i];
		val/=10;
	}
	
	for(i=0;i<6;++i)
	{
		#if (defined(__C16__) && defined(C16_UNEXPANDED))
		_DISPLAY(x+i,y, screenCode((unsigned char) (digits[5-i])+48));			
		#else
		_DISPLAY(x+i,y, (unsigned char) (digits[5-i])+48);
		#endif
	}
}	

void print_02u(unsigned char x, unsigned char y, unsigned short val)
{
	#if (defined(__C16__) && defined(C16_UNEXPANDED))
	_DISPLAY(x,y, screenCode(((unsigned char) val)/10+48));
	_DISPLAY(1+x,y, screenCode(((unsigned char) val)%10+48));		
	#else
	_DISPLAY(x,y, ((unsigned char) val)/10+48);
	_DISPLAY(1+x,y, ((unsigned char) val)%10+48);
	#endif
}	


void print_u(unsigned char x, unsigned char y, unsigned short val)
{
	#if (defined(__C16__) && defined(C16_UNEXPANDED))
	_DISPLAY(x,y, screenCode((unsigned char) (val+48)));		
	#else
	_DISPLAY(x,y, (unsigned char) (val+48));
	#endif
}


void PRINTF(unsigned char x, unsigned char y, char * str, unsigned short val)
{
	if(strlen(str)==5)
	{	
		print_05u0(x,y,val);
	}
	else if(strlen(str)==4)
	{
		print_02u(x,y,val);		
	}
	else
	{
		print_u(x,y,val);		
	}
}




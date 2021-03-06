
#include "memory_mapped_graphics.h"
#include "memory_mapped_alt_print.h"

#include "standard_libs.h"


#if defined(BUFFERED)	

	#include "display_target_geometry.h"
	
	extern uint8_t video_buffer[YSize+Y_OFFSET][XSize];
#endif


#if defined(CBM_SCREEN_CODES)
	char screenCode(char ch)
	{
		if(ch<64)
		{
			return ch;
		}
		else
		{
			return ch-64;
		}
		return ch;
	}
#elif defined(__COCO__) || defined(__DRAGON__)
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
#elif (defined(__VIC20__) && defined(VIC20_EXP_8K))
	char screenCode(char ch)
	{
		if(ch<64)
		{
			return 128+ch;
		}
		else
		{
			return 64+ch;
		}	
	}	
#elif (defined(__C16__) && defined(C16_UNEXPANDED))
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

	#include "display_macros.h"
	
	#define _DISPLAY(x,y,ch) \
		_draw_ch(x,y,ch);
	
#elif defined(BUFFERED)
	#define _DISPLAY(x,y,ch) \
		video_buffer[(y)][(x)] = (ch)
		
#elif defined(NO_GRAPHICS)
	#define _DISPLAY(x,y,ch)
		
#elif (defined(__C16__) && defined(C16_UNEXPANDED)) 
	#define _DISPLAY(x,y,ch) \
		do \
		{ \
			DISPLAY_POKE((loc(x,y)), screenCode(ch)); \
			DISPLAY_POKE((loc(x,y)-1024), PEEK(0x053B)); \
		} while(0)
#elif (defined(__VIC20__) && defined(VIC20_EXP_8K)) && defined(REDEFINED_CHARS)
	#define _DISPLAY(x,y,ch) \
		do \
		{ \
			DISPLAY_POKE((loc(x,y)), screenCode(ch)); \
			DISPLAY_POKE((0x8400+loc(x,y)), PEEK(0x0286)); \
		} while(0)
#elif defined(__C64__)
	#define _DISPLAY(x,y,ch) \
		do \
		{ \
			DISPLAY_POKE((loc(x,y)), screenCode(ch)); \
			DISPLAY_POKE((0x1800+loc(x,y)), PEEK(0x0286)); \
		} while(0)
#else
	#define _DISPLAY(x,y,ch) \
		DISPLAY_POKE((loc(x,y)), (ch))
#endif


void PRINT(uint8_t x, uint8_t y, char * str)
{
	uint8_t i = 0;

	while(str[i]!='\0')
	{
		#if defined(CBM_SCREEN_CODES) || defined(__COCO__) || defined(__DRAGON__) 
			_DISPLAY(x+i,y, screenCode(str[i]));
		#else
			_DISPLAY(x+i,y, str[i]);
		#endif
		++i;
	}
}

void print_05u0(uint8_t x, uint8_t y, uint16_t val)
{
	uint8_t i;
	uint8_t digits[6];
	
	digits[0] = 0;
	for(i=1;i<6;++i)
	{
		digits[i] = (uint8_t) ((val)%10);
		val-= digits[i];
		val/=10;
	}
	
	for(i=0;i<6;++i)
	{
		_DISPLAY(x+i,y, (uint8_t) (digits[5-i])+48);
	}
}	

void print_02u(uint8_t x, uint8_t y, uint16_t val)
{
	_DISPLAY(x,y, ((uint8_t) val)/10+48);
	_DISPLAY(1+x,y, ((uint8_t) val)%10+48);
}	


void print_u(uint8_t x, uint8_t y, uint16_t val)
{
	_DISPLAY(x,y, (uint8_t) (val+48));
}


void PRINTF(uint8_t x, uint8_t y, char * str, uint16_t val)
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







#if defined(FULL_GAME) 

#include "level.h"
#include "strategy.h"

#include "display_macros.h"

#if !defined(TINY_GAME) || defined(TURN_BASED)
extern uint16_t loop;
#endif
extern uint8_t level;

extern Character leftHorizontalMissile;
extern Character rightHorizontalMissile;
extern Character player;

extern uint8_t arrowRange;
extern uint8_t arrowYPosition;

extern uint8_t isBossLevel;
extern uint8_t isMissileLevel;
extern uint8_t isOneMissileLevel;

	uint8_t _playerInArrowRange(void)
	{
		return (loop&1 && player._y<=(arrowYPosition+arrowRange) && player._y>=(arrowYPosition-arrowRange));
	}
	
	void _handle_horizontal_missile_from_the_left(void)
	{
		if(leftHorizontalMissile._status)
		{
			deleteHorizontalMissile(&leftHorizontalMissile);
			if(leftHorizontalMissile._x==XSize-2)
			{
				leftHorizontalMissile._x=0;
				leftHorizontalMissile._y =  arrowYPosition;
			}
			else
			{
				ADVANCED_LEFT_MISSILE();
				if(_playerInArrowRange())
				{
					if(player._x>=leftHorizontalMissile._x)
					{
						(void) moveCharacter((uint8_t *)&leftHorizontalMissile+Y_MOVE, (uint8_t *)&player+Y_MOVE);			
					}
				}
			}
			displayHorizontalMissile(&leftHorizontalMissile);
			
			if(playerKilledBy(&leftHorizontalMissile))
			{
				playerDies();
			}
		}		
	}
	
	void _handle_horizontal_missile_from_the_right(void)
	{
		if(rightHorizontalMissile._status)
		{
			deleteHorizontalMissile(&rightHorizontalMissile);
			if(rightHorizontalMissile._x==1)
			{
				rightHorizontalMissile._x= XSize-1;
				rightHorizontalMissile._y = arrowYPosition;
			}
			else
			{
				ADVANCED_RIGHT_MISSILE();
				if(_playerInArrowRange())
				{
					if(player._x<= rightHorizontalMissile._x)	
					{
						(void) moveCharacter((uint8_t *)&rightHorizontalMissile+Y_MOVE, (uint8_t *)&player+Y_MOVE);			
					}
				}
			}
			displayHorizontalMissile(&rightHorizontalMissile);	
			
			if(playerKilledBy(&rightHorizontalMissile))
			{
				playerDies();
			}
		}		
	}
	
	void handle_horizontal_missiles(void)
	{	
		if(isOneMissileLevel)
		{
			arrowYPosition = YSize/2;
			_handle_horizontal_missile_from_the_right();
		}	
		else if(isMissileLevel || isBossLevel)
		{
			arrowYPosition = HORIZONTAL_MISSILE_OFFSET;
			_handle_horizontal_missile_from_the_right();
			
			arrowYPosition = YSize-1-HORIZONTAL_MISSILE_OFFSET; 
			_handle_horizontal_missile_from_the_left();
		}

	}
	
#endif // defined(FULL_GAME)


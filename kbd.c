#include "common.h"
#include "kbd.h"
//#include "beep.h"

volatile uint8_t	key_pressed;

volatile uint8_t	kbd_value_1;
volatile uint8_t	kbd_value_2;

uint32_t	key_counter[KEY_COUNT];
int			key_fastmode_counter[KEY_COUNT];

void kbd_init(void)
{
	CLEARBIT(KBD_DDR, KBD_BIT);

	kbd_value_1 = 0xFF;
	kbd_value_2 = 0xFF;
	key_pressed = 0x00;
}


void kbd_scan(int key_id)
{
	key_id &= 3; // key_id > 3 or key_id <0 not allowed
	
	if (TESTBIT(kbd_value_1, key_id) && 
		TESTBIT(kbd_value_2, key_id) &&
		!TESTBIT(KBD_PORT, KBD_BIT))
			
		CLEARBIT(kbd_value_1, key_id);
			
	else if (!TESTBIT(kbd_value_1, key_id) && // key pressed
		TESTBIT(kbd_value_2, key_id) &&
		!TESTBIT(KBD_PORT, KBD_BIT))
	{
		CLEARBIT(kbd_value_2, key_id);
		key_counter[key_id] = 0;
	}
	else if (TESTBIT(kbd_value_1, key_id) && 
		!TESTBIT(kbd_value_2, key_id) &&
		!TESTBIT(KBD_PORT, KBD_BIT))
			 
		CLEARBIT(kbd_value_1, key_id);

	else if (!TESTBIT(kbd_value_1, key_id) && // key still pressed
		!TESTBIT(kbd_value_2, key_id) &&
		!TESTBIT(KBD_PORT, KBD_BIT))
	{	 
		if (key_fastmode_counter[key_id] > KBD_FASTMODE_DELAY)
		{	
			if (++key_counter[key_id] > KBD_BIG_DELAY + KBD_VERY_SMALL_DELAY)
			{
				key_counter[key_id] = KBD_BIG_DELAY;
				SETBIT(key_pressed, key_id);
			}
		}
		else
			if (++key_counter[key_id] > KBD_BIG_DELAY + KBD_SMALL_DELAY)
			{
				key_counter[key_id] = KBD_BIG_DELAY;
				key_fastmode_counter[key_id]++;
				SETBIT(key_pressed, key_id);
			}
	}
	else if (TESTBIT(kbd_value_1, key_id) && 
		TESTBIT(kbd_value_2, key_id) &&
		TESTBIT(KBD_PORT, KBD_BIT))
	{
	}	
	else if (!TESTBIT(kbd_value_1, key_id) && 
		TESTBIT(kbd_value_2, key_id) &&
		TESTBIT(KBD_PORT, KBD_BIT))
			 
		SETBIT(kbd_value_1, key_id);
		
	else if (!TESTBIT(kbd_value_1, key_id) && 
		!TESTBIT(kbd_value_2, key_id) &&
		TESTBIT(KBD_PORT, KBD_BIT))
		 
		SETBIT(kbd_value_1, key_id);
			
	else if (TESTBIT(kbd_value_1, key_id) && // key released
		!TESTBIT(kbd_value_2, key_id) &&
		TESTBIT(KBD_PORT, KBD_BIT))
	{	 
		SETBIT(kbd_value_2, key_id);
		SETBIT(key_pressed, key_id);
		key_counter[key_id] = 0;
		key_fastmode_counter[key_id] = 0;
	}
}

void kbd_clear(void)
{
	key_pressed = 0x00;
}

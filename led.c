#include "led.h"

char digits[4];
char font[LED_FONT_SIZE] = {0xD7, 0x14, 0xCD, 0x5D, 0x1E, 0x5B, 0xDB, 0x15, 0xDF, 0x5F, 0x04, 0x00};

void led_init(void)
{
	SETBIT(LED_DDR, LED_CLOCK);
	SETBIT(LED_DDR, LED_DATA);
	SETBIT(LED_DDR, LED_LATCH); 

	CLEARBIT(LED_PORT, LED_CLOCK);
	CLEARBIT(LED_PORT, LED_DATA);
	CLEARBIT(LED_PORT, LED_LATCH); 
	
}

void shiftout(char d)
{
	int i;
	
	LED_LATCH_OFF;
	
	for (i = 0x80; i > 0; i >>= 1)
	{
		if (i & d)
			LED_DATA_ON;
		else
			LED_DATA_OFF;
			
		LED_CLOCK_ON;
		DELAY;
		LED_CLOCK_OFF;
	}
	
	LED_LATCH_ON;
}

void led_show_digit(int d)
{
	char dot_flag = 0;
	char chr;
	
	d &= 0x03;
	chr = digits[d];
	
	if (d)
	{
		dot_flag = chr & LED_DOT;
		chr &= ~LED_DOT;
				
		if (chr >= LED_FONT_SIZE)
			chr = 0;
			
		chr = font[chr];

		if (dot_flag)
			chr |= LED_DOT;
	}
	
	shiftout(chr);
}

void led_show_int(int n)
{
	char	d3, d2, d1;
	
	if ((0 > n) || (n > 999))
	{
		digits[3] = 9 | LED_DOT;
		digits[2] = 9 | LED_DOT;
		digits[1] = 9 | LED_DOT;
		return;
	}
	
	d3 = n / 100;
	if (!d3)
		d3 = LED_SPACE;
		
	n %= 100;

	d2 = n / 10;
	if (!d2 && (LED_SPACE == d3))
		d2 = LED_SPACE;
		
	d1 = n % 10;
	
	digits[3] = d3;
	digits[2] = d2;
	digits[1] = d1;
}

void led_off(int led)
{
	led_set(led, LED_OFF);
}

void led_set(int led, int color)
{
	color &= 3;
	
	switch (led)
	{
	case 0:
		if (color & LED_GREEN)
			SETBIT(digits[0], 2);
		else
			CLEARBIT(digits[0], 2);

		if (color & LED_RED)
			SETBIT(digits[0], 1);
		else
			CLEARBIT(digits[0], 1);

		break;
	
	case 1:
		if (color & LED_GREEN)
			SETBIT(digits[0], 0);
		else
			CLEARBIT(digits[0], 0);

		if (color & LED_RED)
			SETBIT(digits[0], 3);
		else
			CLEARBIT(digits[0], 3);

		break;
		
	case 2:
		if (color & LED_GREEN)
			SETBIT(digits[0], 4);
		else
			CLEARBIT(digits[0], 4);

		if (color & LED_RED)
			SETBIT(digits[0], 5);
		else
			CLEARBIT(digits[0], 5);

		break;
		
	case 3:
		if (color & LED_GREEN)
			SETBIT(digits[0], 6);
		else
			CLEARBIT(digits[0], 6);

		if (color & LED_RED)
			SETBIT(digits[0], 7);
		else
			CLEARBIT(digits[0], 7);

		break;
		
	default:
		break;
	}
}

int led_get(int led)
{
	int color = 0;
	
	switch (led)
	{
	case 0:
		if (TESTBIT(digits[0], 2))
			color |= LED_GREEN;

		if (TESTBIT(digits[0], 1))
			color |= LED_RED;

		break;
	
	case 1:
		if (TESTBIT(digits[0], 0))
			color |= LED_GREEN;

		if (TESTBIT(digits[0], 3))
			color |= LED_RED;

		break;
		
	case 2:
		if (TESTBIT(digits[0], 4))
			color |= LED_GREEN;

		if (TESTBIT(digits[0], 5))
			color |= LED_RED;

		break;
		
	case 3:
		if (TESTBIT(digits[0], 6))
			color |= LED_GREEN;

		if (TESTBIT(digits[0], 7))
			color |= LED_RED;

		break;
		
	default:
		break;
	}
	
	return color;
}

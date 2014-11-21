#if !defined _LED_INCLUDED
	#define _LED_INCLUDED
	
#include "common.h"

#define LED_PORT		PORTC
#define LED_DDR			DDRC

#define LED_CLOCK		(2)
#define	LED_DATA		(3)
#define LED_LATCH		(4)

#define LED_COUNT		(4)
#define LED_FONT_SIZE	(12)

#define LED_CLOCK_ON	SETBIT(LED_PORT, LED_CLOCK)
#define	LED_CLOCK_OFF	CLEARBIT(LED_PORT, LED_CLOCK)

#define LED_DATA_ON		SETBIT(LED_PORT, LED_DATA)
#define	LED_DATA_OFF	CLEARBIT(LED_PORT, LED_DATA)

#define LED_LATCH_ON	SETBIT(LED_PORT, LED_LATCH)
#define	LED_LATCH_OFF	CLEARBIT(LED_PORT, LED_LATCH)

#define LED_OFF			(0)
#define LED_GREEN		(1)
#define LED_RED			(2)
#define LED_YELLOW		(3)

void led_init(void);
void led_show_digit(int);
void led_show_int(int);
void led_set(int, int);
void led_off(int);
int led_get(int);

extern char digits[LED_COUNT];
//extern char font[LED_FONT_SIZE];

#define	LED_MINUS	(10)
#define LED_SPACE	(11)
#define	LED_DOT		(0x20)

#endif /* _LED_INCLUDED */
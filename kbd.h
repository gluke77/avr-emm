#if !defined _KBD_INCLUDED
	#define _KBD_INCLUDED
	
#include "common.h"

#define KBD_PORT	PINC
#define KBD_DDR		DDRC
#define KBD_BIT		(1)

#define KEY_COUNT		(4)
#define KBD_TIMEOUT		(11)

#define KBD_BIG_DELAY			(150)
#define KBD_SMALL_DELAY			(10)
#define KBD_VERY_SMALL_DELAY	(2)

#define KBD_FASTMODE_DELAY		(50)

#define KEY_SELECT	(3)
#define KEY_DOWN	(2)
#define KEY_UP		(1)
#define KEY_RUNSTOP	(0)

void kbd_scan(int);
void kbd_init(void);
void kbd_clear(void);

#define KEY_PRESSED(key_id)			TESTBIT(key_pressed, key_id)
#define CLEAR_KEY_PRESSED(key_id)	CLEARBIT(key_pressed, key_id)
#define SET_KEY_PRESSED(key_id)		SETBIT(key_pressed, key_id)

extern volatile uint8_t	key_pressed;
	
#endif /* _KBD_INCLUDED */
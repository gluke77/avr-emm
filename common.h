#if !defined __COMMON_H_INCLUDED_
#define __COMMON_H_INCLUDED_

#define TRUE 1
#define FALSE 0

#define FOSC	(F_CPU)

#define TESTBIT(port, bit)	(port & _BV(bit))
#define SETBIT(port, bit)	(port |= _BV(bit))
#define CLEARBIT(port, bit)	(port &= ~(_BV(bit)))

#define TESTBITL(port, bit)		((port) & (1L << (bit)))
#define SETBITL(port, bit)		((port) |= (1L << (bit)))
#define CLEARBITL(port, bit)	((port) &= ~(1L << (bit)))

#define	EXT_MEM_INIT	{MCUCR = (1<<SRE);} // 
			//			SETBIT(XMCRB, XMM0); 
			//			SETBIT(XMCRB, XMM1); 
			//			SETBIT(XMCRB, XMM2);}

//#define	GLOBAL_INT_ENABLE   SETBIT( SREG, 7 )
//#define	GLOBAL_INT_DISABLE  CLEARBIT( SREG, 7 )

#define	GLOBAL_INT_ENABLE   sei()
#define	GLOBAL_INT_DISABLE  cli()

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <inttypes.h>
#include <util/delay.h>

#define ADDR_PORT	PORTD
#define ADDR_DDR	DDRD

#define ADDR_BIT0	(7)
#define ADDR_BIT1	(6)
#define	ADDR_BIT2	(5)
#define ADDR_BIT3	(4)

void	addr_init(void);
void	addr_set(int);

extern char	address;

#define DELAY {int i = 20; while (--i);}


#endif /* __COMMON_H_INCLUDED_*/

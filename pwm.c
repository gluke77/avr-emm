#include "pwm.h"

int g_run[2];
int g_pwm[2];

void pwm_init(void)
{
	ICR1 = PWM_TOP;
	
	SETBIT(TCCR1A, COM1A1);	// inverted pwm
	//SETBIT(TCCR1A, COM1A0);	// direct pwm
	
	SETBIT(TCCR1A, COM1B1);	// inverted pwm
	//SETBIT(TCCR1A, COM1B0);	// direct pwm
	
	SETBIT(TCCR1B, WGM13);  // phase correct
	SETBIT(TCCR1A, WGM11);  //

	SETBIT(TCCR1B,CS10);	// no prescaling
	
	SETBIT(DDRB, 1);
	SETBIT(DDRB, 2);
	
	g_pwm[0] = 0;
	g_pwm[1] = 0;
	
	pwm_off(0);
	pwm_off(1);
}

void pwm_set(int ch, int pwm)
{
	if (0 > pwm)
		pwm = 0;
		
	if (pwm > PWM_TOP)
		pwm = PWM_TOP;

	char sreg = SREG;
	GLOBAL_INT_DISABLE;
	if (ch & 1)
		OCR1B = PWM_TOP - pwm;
	else
		OCR1A = PWM_TOP - pwm;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
	SREG = sreg;
}

int pwm_get_value(int ch)
{
	return g_pwm[ch & 1];
}

void pwm_set_value(int ch, int pwm)
{
	ch &= 1;
	
	if (0 > pwm)
		pwm = 0;
		
	if (pwm > PWM_TOP)
		pwm = PWM_TOP;

	g_pwm[ch] = pwm;
	
	if (pwm_isrun(ch))
		pwm_set(ch, g_pwm[ch]);
}
		
void pwm_off(int ch)
{
	ch &= 1;
	
	g_run[ch] = 0;
	pwm_set(ch, 0);
}

void pwm_on(int ch)
{
	ch &= 1;
	
	g_run[ch] = 1;
	pwm_set(ch, g_pwm[ch]);
}

int pwm_isrun(int ch)
{
	return g_run[ch];
}

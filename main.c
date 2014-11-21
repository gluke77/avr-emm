#include "common.h"

#include <stdio.h>

#include "timer.h"
#include "kbd.h"
#include "led.h"
#include "usart.h"
#include "pwm.h"
#include "adc.h"
#include "modbus.h"

#define BLINK_DELAY			(1000)
#define BLINK_START_DELAY	(10000)

#define ADC0_ADDR		(0)
#define ADC1_ADDR		(2)

char g_addr;
int g_channel;

int g_blink_mode = 0;
int g_blinker = 0;
int g_start_blink_delay = 0;

uint8_t g_modbus_id = 0;

#define ADC_TOP		(500)
#define ADC_DELTA	(2)

int	g_adc[2];
int g_adjust_mode[2];

//void print_adc(void);
void usart0_puts(char *);
void process_kbd(void);
void setup_leds(void);
void adjust_pwm(void);
void process_usart(void);

int main(void)
{
	addr_init();
	kbd_init();
	led_init();
	timer_init();
	pwm_init();
	adc_init(ADC_NOT_USE_INTERRUPT);
	usart0_init(USART_RS485_SLAVE, 19200);
	usart0_setprotocol_modbus();
	
	GLOBAL_INT_ENABLE;

	g_adc[0] = eeprom_read_word(ADC0_ADDR);
	g_adc[1] = eeprom_read_word(ADC1_ADDR);
	
	for(;;)
	{
		process_kbd();
		setup_leds();
		adjust_pwm();
		process_usart();
		//print_adc();
		//usart0_putchar('1');
	}

	return 0;
}

void addr_init(void)
{
	SETBIT(ADDR_DDR, ADDR_BIT0);
	SETBIT(ADDR_DDR, ADDR_BIT1);
	SETBIT(ADDR_DDR, ADDR_BIT2);
	SETBIT(ADDR_DDR, ADDR_BIT3);
	
	g_addr = 0;
	addr_set(g_addr);
}

void addr_set(int addr)
{
	SETBIT(ADDR_PORT, ADDR_BIT0);
	SETBIT(ADDR_PORT, ADDR_BIT1);
	SETBIT(ADDR_PORT, ADDR_BIT2);
	SETBIT(ADDR_PORT, ADDR_BIT3);
	
	switch (addr)
	{
	case 0:
		CLEARBIT(ADDR_PORT, ADDR_BIT0);
		break;
	
	case 1:
		CLEARBIT(ADDR_PORT, ADDR_BIT1);
		break;

	case 2:
		CLEARBIT(ADDR_PORT, ADDR_BIT2);
		break;

	case 3:
		CLEARBIT(ADDR_PORT, ADDR_BIT3);
		break;
	
	default:
		break;
	}
}

void do_timer(void)
{
	static int blink_delay = 0;

	kbd_scan(g_addr);
	if (++g_addr > 3)
		g_addr = 0;

	led_show_digit(g_addr);
	addr_set(g_addr);
	
	if (++blink_delay > BLINK_DELAY)
	{
		blink_delay = 0;
		g_blinker ^= 1;
	}
	
	if (pwm_isrun(0) && pwm_isrun(1) && !g_blink_mode)
	{
		if (++g_start_blink_delay > BLINK_START_DELAY)
		{
			g_blink_mode = 1;
			g_start_blink_delay = 0;
		}
	}
}
/*
#define PRINT_DELAY	(5000)
void print_adc(void)
{
	char buf[50];
	int adc0;
	int adc1;
	static uint32_t	delay = PRINT_DELAY;

	if (!--delay)
	{
		delay = PRINT_DELAY;

		adc0 = adc_single_value(6);
		adc1 = adc_single_value(7);

		sprintf(buf, "adc0 = %04d\tadc1 = %04d\n", adc0, adc1);
		usart0_puts(buf);
	}
}
*/
/*
void usart0_puts(char * buf)
{
	while (*buf)
		usart0_putchar(*buf++);
}
*/
void process_kbd(void)
{
	if (KEY_PRESSED(KEY_SELECT))
	{
		CLEAR_KEY_PRESSED(KEY_SELECT);
		
		if (g_blink_mode)
		{
			g_blink_mode = 0;
			return;
		}
		g_start_blink_delay = 0;
		
		g_channel ^= 1;
	}

	if (KEY_PRESSED(KEY_UP))
	{
		CLEAR_KEY_PRESSED(KEY_UP);
		
		if (g_blink_mode)
		{
			g_blink_mode = 0;
			return;
		}
		g_start_blink_delay = 0;
		
		if (g_adc[g_channel] < ADC_TOP)
		{
			g_adc[g_channel]++;
			
			eeprom_write_word((g_channel)?ADC1_ADDR:ADC0_ADDR, g_adc[g_channel]);
		}
	}

	if (KEY_PRESSED(KEY_DOWN))
	{
		CLEAR_KEY_PRESSED(KEY_DOWN);
			
		if (g_blink_mode)
		{
			g_blink_mode = 0;
			return;
		}
		g_start_blink_delay = 0;

		if (g_adc[g_channel] > 0)
		{
			g_adc[g_channel]--;
			
			eeprom_write_word((g_channel)?ADC1_ADDR:ADC0_ADDR, g_adc[g_channel]);
		}

	}
		
	if (KEY_PRESSED(KEY_RUNSTOP))
	{
		CLEAR_KEY_PRESSED(KEY_RUNSTOP);

		if (g_blink_mode)
		{
			g_blink_mode = 0;
			return;
		}
		g_start_blink_delay = 0;
	
		if (pwm_isrun(g_channel))
			pwm_off(g_channel);
		else
			pwm_on(g_channel);
	}
}

void setup_leds(void)
{
	if (g_adjust_mode[0] && pwm_isrun(0))
		led_set(2, (g_blinker)?LED_GREEN:LED_RED);
	else
		led_set(2, (pwm_isrun(0))?LED_GREEN:LED_OFF);
	
	if (g_adjust_mode[1] && pwm_isrun(1))
		led_set(3, (g_blinker)?LED_GREEN:LED_RED);
	else
		led_set(3, (pwm_isrun(1))?LED_GREEN:LED_OFF);

	if (g_blink_mode)
	{
		led_set(0, (g_blinker)?LED_OFF:LED_GREEN);
		led_set(1, (g_blinker)?LED_GREEN:LED_OFF);
		
		//led_show_int(pwm_get_value(g_blinker));
		led_show_int(g_adc[g_blinker]);
	}
	else
	{
		led_set(0, (g_channel)?LED_OFF:LED_RED);
		led_set(1, (g_channel)?LED_RED:LED_OFF);
	
		led_show_int(g_adc[g_channel]);
	}
}

void adjust_pwm(void)
{
	int ch;
	
	for (ch = 0; ch < 2; ch++)
		if (pwm_isrun(ch))
		{
			if (g_adc[ch] < adc_single_value(6 + ch))
			{
				pwm_set_value(ch, pwm_get_value(ch) - 1);
				g_adjust_mode[ch] = 1;
			}
			else if (g_adc[ch] > adc_single_value(6 + ch))
			{
				pwm_set_value(ch, pwm_get_value(ch) + 1);
				g_adjust_mode[ch] = 1;
			}
			else
				g_adjust_mode[ch] = 0;
		}
}

void process_usart(void)
{
	uint16_t		value;
	modbus_cmd_s	cmd;
	result_e		res;
	uint8_t			msg[MODBUS_MAX_MSG_LENGTH];

	if (!usart0_msg_ready)
		return;
		
	usart0_msg_ready = 0;
	res = modbus_msg2cmd(usart0_inbuf, &cmd);
			
	if (RESULT_OK == res)
		if (g_modbus_id == cmd.device_id)
		{
			if (MODBUS_WRITE == cmd.cmd_code)
			{
				value = cmd.value[0];
				switch (cmd.addr)
				{
				case 0x0000:
					switch (value & 0x0003)
					{
					case 0x0001:
						pwm_off(0);
						break;
					case 0x0002:
						pwm_on(0);
						break;
					default:
						break;
					}
						
					switch (value & 0x000C)
					{
					case 0x0004:
						pwm_off(1);
						break;
					case 0x0008:
						pwm_on(1);
						break;
					default:
						break;
					}

					break;

				case 0x0001:	// set adc0
					if (value > ADC_TOP)
						value = ADC_TOP;
						
					if (value < 0)
						value = 0;
						
					g_adc[0] = value;
					
					eeprom_write_word(ADC0_ADDR, g_adc[0]);
					break;

				case 0x0002:	// set adc0
					if (value > ADC_TOP)
						value = ADC_TOP;
						
					if (value < 0)
						value = 0;
						
					g_adc[1] = value;
					
					eeprom_write_word(ADC1_ADDR, g_adc[1]);
					break;
				
				default:
					break;
				}
							
				modbus_cmd2msg(&cmd, msg, MODBUS_MAX_MSG_LENGTH);
				usart0_cmd(msg, 0, 0, 300);
			}
			else if (MODBUS_READ == cmd.cmd_code)
			{
				cmd.device_id = g_modbus_id;
				cmd.cmd_code = MODBUS_READ;
				cmd.cmd_type = MODBUS_ACK;

				if (0x0000 == cmd.addr)
				{
					cmd.addr = 7;
				
					cmd.value[0] = (pwm_isrun(0))?2:1;
					cmd.value[0] |= ((pwm_isrun(1))?2:1) << 2;

					cmd.value[1] = g_adc[0];
					cmd.value[2] = g_adc[1];
					cmd.value[3] = adc_single_value(6);
					cmd.value[4] = adc_single_value(7);
					cmd.value[5] = pwm_get_value(0);
					cmd.value[6] = pwm_get_value(1);
				}
			
				modbus_cmd2msg(&cmd, msg, MODBUS_MAX_MSG_LENGTH);
				usart0_cmd(msg, 0, 0, 300);
			}
		}
}

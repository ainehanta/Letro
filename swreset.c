/*
 * swreset.c
 *
 * Created: 2012/09/05 16:23:00
 *  Author: makoto
 */ 

//“Á‚É‚¢‚¶‚ç‚È‚¢‚Å‚­‚¾‚³‚¢

#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "lcd.h"
#include "led.h"
#include "swreset.h"

char msg[20];

const int8_t _mes_sw_menu_0[17] PROGMEM = "SoftResetMenu  ";
const int8_t _mes_sw_menu_1_ok[17] PROGMEM = "Exit? *OK  NO   ";
const int8_t _mes_sw_menu_1_no[17] PROGMEM = "Exit?  OK *NO   ";

extern uint8_t _exit_flag;

void sw_reset_init(void)
{
	SW_RESET_DDR &= ~(_BV(SW_RESET_BTN));
	SW_RESET_PORT |= _BV(SW_RESET_BTN);
	PCICR |= 0b00000010;
	PCMSK1 |= 0b00010000;
}

void sw_reset_enable(void)
{
	sei();
}

void sw_reset_disable(void)
{
	PCMSK1 &= ~(0b00010000);
}

void sw_reset_menu(void)
{
	strcpy_P(msg,_mes_sw_menu_0);
	lcd_put_data(0,msg);
	strcpy_P(msg,_mes_sw_menu_1_ok);
	lcd_put_data(1,msg);
	_exit_flag = 1;
}

ISR(PCINT1_vect)
{
	uint8_t sreg;
	sreg = SREG;
	_delay_ms(1);
	sw_reset_menu();
	sei();
	SREG = sreg;
}
/*
 * main.c
 *
 *  Created on: 2012/08/12
 *      Author: makoto
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>

#include "app.h"
#include "lcd.h"
#include "led.h"
#include "menu.h"
#include "sound.h"
#include "switch.h"
#include "swreset.h"

#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

uint8_t _exit_flag = 0;
int8_t _main_mes[17];

int main(void)
{
	lcd_init();
	led_init();
	menu_init();
	sw_reset_init();
	switch_init();
	
	sw_reset_enable();

	while(1)
	{
		menu();
	}
}

ISR(PCINT1_vect)
{
	_delay_ms(1);
	sw_reset_menu();
	sei();
}

/*

if(switch_get(SWITCH_PIN0,SWITCH_P0_A))
{
}

if(switch_get(SWITCH_PIN0,SWITCH_P0_B))
{
}

if(switch_get(SWITCH_PIN0,SWITCH_P0_U))
{
}

if(switch_get(SWITCH_PIN0,SWITCH_P0_D))
{
}

if(switch_get(SWITCH_PIN0,SWITCH_P0_L))
{
}

if(switch_get(SWITCH_PIN0,SWITCH_P0_R))
{
}

if(switch_get(SWITCH_PIN0,SWITCH_P1_A))
{
}

if(switch_get(SWITCH_PIN0,SWITCH_P1_B))
{
}

if(switch_get(SWITCH_PIN1,SWITCH_P1_D))
{
}

if(switch_get(SWITCH_PIN1,SWITCH_P1_L))
{
}

if(switch_get(SWITCH_PIN1,SWITCH_P1_R))
{
}

if(switch_get(SWITCH_PIN1,SWITCH_P1_U))
{
}

*/
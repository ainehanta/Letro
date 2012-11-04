/*
 * main.c
 *
 *  Created on: 2012/08/12
 *      Author: makoto
 */

//特にいじらないでください

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>

#include "app.h"
#include "lcd.h"
#include "led.h"
#include "menu.h"
#include "switch.h"
#include "swreset.h"

#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#ifndef _BUILD_
	#include <avr/iom1284p.h>
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

	while(1)
	{
		menu();
	}
}


/*

switch_get(SWITCH_CONT_P0,&switch_state_p0);
if((switch_state_p0.switch_a==1)&&(switch_state_p0.switch_prev_a==0))
{
}
else if((switch_state_p0.switch_b==1)&&(switch_state_p0.switch_prev_b==0))
{
}

if((switch_state_p0.switch_u==1)&&(switch_state_p0.switch_prev_u==0))
{
}
else if((switch_state_p0.switch_d==1)&&(switch_state_p0.switch_prev_d==0))
{
}
else if((switch_state_p0.switch_l==1)&&(switch_state_p0.switch_prev_l==0))
{
}
else if((switch_state_p0.switch_r==1)&&(switch_state_p0.switch_prev_r==0))
{
}

switch_get(SWITCH_CONT_P1,&switch_state_p1);
if((switch_state_p1.switch_a==1)&&(switch_state_p1.switch_prev_a==0))
{
}
else if((switch_state_p1.switch_b==1)&&(switch_state_p1.switch_prev_b==0))
{
}

if((switch_state_p1.switch_u==1)&&(switch_state_p1.switch_prev_u==0))
{
}
else if((switch_state_p1.switch_d==1)&&(switch_state_p1.switch_prev_d==0))
{
}
else if((switch_state_p1.switch_l==1)&&(switch_state_p1.switch_prev_l==0))
{
}
else if((switch_state_p1.switch_r==1)&&(switch_state_p1.switch_prev_r==0))
{
}

*/

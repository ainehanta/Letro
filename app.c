/*
 * app.c
 *
 * Created: 2012/09/05 22:05:40
 *  Author: makoto
 */ 

#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "app.h"
#include "lcd.h"
#include "led.h"
#include "switch.h"
#include "sound.h"

extern uint8_t _exit_flag;

void demo(void)
{
	sw_reset_enable();
	
	lcd_put_data(0,"Running Demo... ");
	lcd_put_data(1,"                ");
	
	uint8_t prev_dot_p0[2];
	switch_state switch_state_p0;
	switch_state_clear(&switch_state_p0);
	prev_dot_p0[0] = prev_dot_p0[1] = 0;
	uint8_t led_color_p0 = LED_GREEN;
	
	uint8_t prev_dot_p1[2];
	switch_state switch_state_p1;
	switch_state_clear(&switch_state_p1);
	prev_dot_p1[0] = prev_dot_p1[1] = 0;
	uint8_t led_color_p1 = LED_GREEN;
	
	for(;_exit_flag==0;)
	{
		switch_get(SWITCH_CONT_P0,&switch_state_p0);
		if((switch_state_p0.switch_a==1)&&(switch_state_p0.switch_prev_a==0))
		{
			if(led_color_p0 == LED_GREEN)
			{
				led_color_p0 = LED_RED;
			}
			else if(led_color_p0 == LED_RED)
			{
				led_color_p0 = LED_YELLOW;
			}
			else if(led_color_p0 == LED_YELLOW)
			{
				led_color_p0 = LED_GREEN;
			}
		}
		else if((switch_state_p0.switch_b==1)&&(switch_state_p0.switch_prev_b==0))
		{
		}

		if((switch_state_p0.switch_u==1)&&(switch_state_p0.switch_prev_u==0))
		{
			if(prev_dot_p0[1]>0)
				prev_dot_p0[1]--;
		}
		else if((switch_state_p0.switch_d==1)&&(switch_state_p0.switch_prev_d==0))
		{
			if(prev_dot_p0[1]<15)
				prev_dot_p0[1]++;
		}
		else if((switch_state_p0.switch_l==1)&&(switch_state_p0.switch_prev_l==0))
		{
			if(prev_dot_p0[0]>0)
				prev_dot_p0[0]--;
		}
		else if((switch_state_p0.switch_r==1)&&(switch_state_p0.switch_prev_r==0))
		{
			if(prev_dot_p0[0]<15)
				prev_dot_p0[0]++;
		}
		
		switch_get(SWITCH_CONT_P1,&switch_state_p1);
		if((switch_state_p1.switch_a==1)&&(switch_state_p1.switch_prev_a==0))
		{
			if(led_color_p1 == LED_GREEN)
			{
				led_color_p1 = LED_RED;
			}
			else if(led_color_p1 == LED_RED)
			{
				led_color_p1 = LED_YELLOW;
			}
			else if(led_color_p1 == LED_YELLOW)
			{
				led_color_p1 = LED_GREEN;
			}
		}
		else if((switch_state_p1.switch_b==1)&&(switch_state_p1.switch_prev_b==0))
		{
		}

		if((switch_state_p1.switch_u==1)&&(switch_state_p1.switch_prev_u==0))
		{
			if(prev_dot_p1[1]>0)
			prev_dot_p1[1]--;
		}
		else if((switch_state_p1.switch_d==1)&&(switch_state_p1.switch_prev_d==0))
		{
			if(prev_dot_p1[1]<15)
				prev_dot_p1[1]++;
		}
		else if((switch_state_p1.switch_l==1)&&(switch_state_p1.switch_prev_l==0))
		{
			if(prev_dot_p1[0]>0)
				prev_dot_p1[0]--;			
		}
		else if((switch_state_p1.switch_r==1)&&(switch_state_p1.switch_prev_r==0))
		{
			if(prev_dot_p1[0]<15)
				prev_dot_p1[0]++;
		}
		
		
		led_plot(led_color_p0,prev_dot_p0[0],prev_dot_p0[1]);
		led_plot(led_color_p1,prev_dot_p1[0],prev_dot_p1[1]);
		
		led_draw_line();
		
		_delay_us(100);
	}
}

void test(void)
{
	
}

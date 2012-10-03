/*
 * switch.c
 *
 * Created: 2012/09/05 16:18:56
 *  Author: makoto
 */ 

//特にいじらないでください

#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#ifndef _BUILD_
	#include <avr/iom1284p.h>
#endif

#include <avr/io.h>
#include <util/delay.h>

#include "switch.h"

void switch_state_clear(switch_state *target)
{
	target->switch_a=0;
	target->switch_b=0;
	target->switch_u=0;
	target->switch_d=0;
	target->switch_l=0;
	target->switch_r=0;
	target->switch_prev_a=0;
	target->switch_prev_b=0;
	target->switch_prev_u=0;
	target->switch_prev_d=0;
	target->switch_prev_l=0;
	target->switch_prev_r=0;
}

void switch_init(void)
{
	SWITCH_DDR0 &= ~(SWITCH_PINS0);
	SWITCH_DDR1 &= ~(SWITCH_PINS1);
	
	SWITCH_PORT0 |= SWITCH_PINS0;
	SWITCH_PORT1 |= SWITCH_PINS1;
}

void switch_get(uint8_t cnt,switch_state* target)
{
	uint8_t ret = 0;
	
	target->switch_prev_a = target->switch_a;
	target->switch_prev_b = target->switch_b;
	target->switch_prev_u = target->switch_u;
	target->switch_prev_d = target->switch_d;
	target->switch_prev_l = target->switch_l;
	target->switch_prev_r = target->switch_r;
	
	if(cnt==SWITCH_CONT_P0)
	{
		if(_SWITCH_PIN0&1<<SWITCH_P0_A) {target->switch_a = 0;} else {target->switch_a = 1;}
		if(_SWITCH_PIN0&1<<SWITCH_P0_B) {target->switch_b = 0;} else {target->switch_b = 1;}
		if(_SWITCH_PIN0&1<<SWITCH_P0_U) {target->switch_u = 0;} else {target->switch_u = 1;}
		if(_SWITCH_PIN0&1<<SWITCH_P0_D) {target->switch_d = 0;} else {target->switch_d = 1;}
		if(_SWITCH_PIN0&1<<SWITCH_P0_L) {target->switch_l = 0;} else {target->switch_l = 1;}
		if(_SWITCH_PIN0&1<<SWITCH_P0_R) {target->switch_r = 0;} else {target->switch_r = 1;}
	}
	else if(cnt==SWITCH_CONT_P1)
	{
		if(_SWITCH_PIN0&1<<SWITCH_P1_A) {target->switch_a = 0;} else {target->switch_a = 1;}
		if(_SWITCH_PIN0&1<<SWITCH_P1_B) {target->switch_b = 0;} else {target->switch_b = 1;}
		if(_SWITCH_PIN1&1<<SWITCH_P1_U) {target->switch_u = 0;} else {target->switch_u = 1;}
		if(_SWITCH_PIN1&1<<SWITCH_P1_D) {target->switch_d = 0;} else {target->switch_d = 1;}
		if(_SWITCH_PIN1&1<<SWITCH_P1_L) {target->switch_l = 0;} else {target->switch_l = 1;}
		if(_SWITCH_PIN1&1<<SWITCH_P1_R) {target->switch_r = 0;} else {target->switch_r = 1;}
	}
}

void switch_get_reset(void)
{
	
}

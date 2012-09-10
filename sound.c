/*
 * sound.c
 *
 * Created: 2012/09/05 16:19:43
 *  Author: makoto
 */ 

//“Á‚É‚¢‚¶‚ç‚È‚¢‚Å‚­‚¾‚³‚¢

#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "sound.h"

uint8_t sound_count;
uint8_t sound_status;

void sound_init(void)
{
	SOUND_DDR |= SOUND_PINS;
	TCCR1A = 0;
	TCCR1B = SOUND_T1_PRESCALER;
}

void sound_disable(void)
{
	TIMSK1 &= ~(_BV(TOIE1));
	SOUND_PORT &= ~SOUND_PINS;
}

uint8_t sound_check(void)
{
	return sound_count;
}

void sound_set(uint8_t hz,uint8_t count)
{
	if(hz < SOUND_HZ_MIN) hz = SOUND_HZ_MIN;
	if(hz > SOUND_HZ_MAX) hz = SOUND_HZ_MAX;
	
	sound_status = (uint8_t)(0x100-(SOUND_HZ_MAX/hz));
	sound_count = (hz/10)*count;
	
	TCNT1 = sound_status;
	TIFR1 |= 1;
	TIMSK1 |= _BV(TOIE1);
}

ISR(TIMER1_OVF_vect)
{
	uint8_t sreg;
	sreg = SREG;
	
	TCNT1 = sound_status;
	
	SOUND_PORT ^= _BV(SOUND_DEVICE_PIEZO);
	
	if(((SOUND_PORT)&_BV(SOUND_DEVICE_PIEZO))==0)
	{
		sound_count--;
		if(sound_count==0)
		{
			TIMSK1 &= ~_BV(TOIE1);
		}
	}
	
	SREG = sreg;
}
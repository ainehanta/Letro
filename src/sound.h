/*
 * sound.h
 *
 * Created: 2012/09/05 16:19:52
 *  Author: makoto
 */ 

//特にいじらないでください

#ifndef SOUND_H_
#define SOUND_H_

#ifndef _BUILD_
	#include <avr/iom1284p.h>
#endif

#define SOUND_A			440
#define SOUND_A_SHARP	466
#define SOUND_B_FLAT	SOUND_A_SHARP
#define SOUND_B			493
#define SOUND_C_FLAT	SOUND_B
#define SOUND_C			523
#define SOUND_B_SHARP	SOUND_C
#define SOUND_C_SHARP	554
#define SOUND_D_FLAT	SOUND_C_SHARP
#define SOUND_D			587
#define SOUND_D_SHARP	622
#define SOUND_E_FLAT	SOUND_D_SHARP
#define SOUND_E			659
#define SOUND_F_FLAT	SOUND_E
#define SOUND_F			698
#define SOUND_E_SHARP	SOUND_F
#define SOUND_F_SHARP	739
#define SOUND_G_FLAT	SOUND_F_SHARP
#define SOUND_G			783
#define SOUND_G_SHARP	830
#define SOUND_A_HIGH_FLAT	SOUND_G_SHARP
#define SOUND_A_HIGH_	880

#define SOUND_T1_PRESCALE	128
#define SOUND_T1_PRESCALER	5

#define SOUND_HZ_MAX		((F_CPU/SOUND_T1_PRESCALE)/2)
#define SOUND_HZ_MIN		(((F_CPU/SOUND_T1_PRESCALE)/256)/2)

#define SOUND_PORT	PORTD
#define SOUND_PINS	(_BV(PORTD4) | _BV(PORTD5))
#define SOUND_DDR	DDRD

#define SOUND_DEVICE_PIEZO			PORTD4
#define SOUND_DEVICE_EXT_SPEAKER	PORTD5

void sound_init(void);
void sound_disable(void);
uint8_t sound_check(void);
void sound_set(uint8_t,uint8_t);


#endif /* SOUND_H_ */

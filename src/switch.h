/*
 * switch.h
 *
 * Created: 2012/09/05 16:19:07
 *  Author: makoto
 */ 

//特にいじらないでください

#ifndef SWITCH_H_
#define SWITCH_H_

#ifndef _BUILD_
	#include <avr/iom1284p.h>
#endif

#define SWITCH_DDR0 DDRA
#define SWITCH_DDR1 DDRD
#define SWITCH_DDR2 DDRB

#define SWITCH_PINS0 0xFF
#define SWITCH_PINS1 (_BV(PIND2)|_BV(PIND3)|_BV(PIND6)|_BV(PIND7))
#define SWITCH_PINS2 (_BV(PINB3))

#define _SWITCH_PIN0 PINA
#define _SWITCH_PIN1 PIND
#define _SWITCH_PIN2 PINB

#define SWITCH_PORT0 PORTA
#define SWITCH_PORT1 PORTD
#define SWITCH_PORT2 PORTB

#define SWITCH_PIN0 0
#define SWITCH_PIN1 1
#define SWITCH_PIN2 2

#define SWITCH_CONT_P0 0
#define SWITCH_CONT_P1 1

#define SWITCH_P0_A	1
#define SWITCH_P0_B 0
#define SWITCH_P0_U 2
#define SWITCH_P0_D 3
#define SWITCH_P0_L 4
#define SWITCH_P0_R 5

#define SWITCH_P1_A 6
#define SWITCH_P1_B 7
#define SWITCH_P1_U 3
#define SWITCH_P1_D 2
#define SWITCH_P1_L 6
#define SWITCH_P1_R 7

typedef struct 
{
	uint8_t switch_a;
	uint8_t switch_b;
	uint8_t switch_u;
	uint8_t switch_d;
	uint8_t switch_l;
	uint8_t switch_r;
	
	uint8_t switch_prev_a;
	uint8_t switch_prev_b;
	uint8_t switch_prev_u;
	uint8_t switch_prev_d;
	uint8_t switch_prev_l;
	uint8_t switch_prev_r;
} switch_state;

void switch_init(void);
void switch_state_clear(switch_state*);
void switch_get(uint8_t,switch_state*);


#endif /* SWITCH_H_ */

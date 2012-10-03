/*
 * swreset.h
 *
 * Created: 2012/09/05 16:23:16
 *  Author: makoto
 */ 

//特にいじらないでください


#ifndef SWRESET_H_
#define SWRESET_H_

#ifndef _BUILD_
	#include <avr/iom1284p.h>
#endif

#define SW_RESET_BTN PORTB4

#define SW_RESET_PIN PINB
#define SW_RESET_PORT PORTB
#define SW_RESET_DDR DDRB

void sw_reset_init(void);
void sw_reset_enable(void);
void sw_reset_disable(void);
void sw_reset_menu(void);


#endif /* SWRESET_H_ */

/*
 * menu.h
 *
 * Created: 2012/09/05 16:07:43
 *  Author: makoto
 */ 


#ifndef MENU_H_
#define MENU_H_

#ifndef _BUILD_
	#include <avr/iom1284p.h>
#endif

#define MENU_DEMO	0
#define MENU_SOUND	1

#define MENU_MAX	1
#define MENU_MIN	2

#define MENU_DEFAULT MENU_DEMO

void menu_init(void);
void menu_exec(uint8_t);
void menu(void);


#endif /* MENU_H_ */

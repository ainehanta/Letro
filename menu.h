/*
 * menu.h
 *
 * Created: 2012/09/05 16:07:43
 *  Author: makoto
 */ 


#ifndef MENU_H_
#define MENU_H_


#define MENU_MAIN	0
#define MENU_SUB	1
#define MENU_DEMO	2
#define MENU_TEST	3

#define MENU_DEFAULT MENU_MAIN

void menu_init(void);
void menu(void);


#endif /* MENU_H_ */
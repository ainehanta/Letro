/*
 * menu.c
 *
 * Created: 2012/09/05 16:07:29
 *  Author: makoto
 */ 

/*

	起動したいアプリケーションのやつを例に習って記述してください。
	多分、組み込める形にする作業はこちらでやるので「app.c」と「app.h」のみ記述してください。

*/

#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include <string.h>

#include "menu.h"
#include "swreset.h"
#include "switch.h"
#include "lcd.h"
#include "app.h"

uint8_t menu_mode;
int8_t _mes_menu[17];

extern uint8_t _exit_flag;

const int8_t _mes_menu_select_top_0[] PROGMEM = "Letro: SelectApp";
const int8_t _mes_menu_select_app_list_1[2][20] PROGMEM = {"01 : Demo       ","02 : Sound Test "};

void menu_init(void)
{
	menu_mode = MENU_DEFAULT;
}

void menu_exec(uint8_t mode)
{
	sw_reset_enable();
	switch(mode)
	{
		case MENU_DEMO  : app_demo();  break;
		case MENU_SOUND : app_sound(); break;
	}
}

void menu(void)
{
	strcpy_P(_mes_menu,_mes_menu_select_top_0);
	lcd_put_data(0,_mes_menu);
	strcpy_P(_mes_menu,_mes_menu_select_app_list_1[MENU_DEFAULT]);
	lcd_put_data(1,_mes_menu);
	
	sw_reset_disable();
	
	switch_state switch_state_p0;
	switch_state_clear(&switch_state_p0);
	switch_state switch_state_p1;
	switch_state_clear(&switch_state_p1);
	
	for(;;)
	{
		if(_exit_flag!=0)
		{
			strcpy_P(_mes_menu,_mes_menu_select_top_0);
			lcd_put_data(0,_mes_menu);
			strcpy_P(_mes_menu,_mes_menu_select_app_list_1[MENU_DEFAULT]);
			lcd_put_data(1,_mes_menu);
			_exit_flag = 0;
		}
		
		switch_get(SWITCH_CONT_P0,&switch_state_p0);
		if((switch_state_p0.switch_a==1)&&(switch_state_p0.switch_prev_a==0))
		{
			menu_exec(menu_mode);
		}
		else if((switch_state_p0.switch_b==1)&&(switch_state_p0.switch_prev_b==0))
		{
		}

		if((switch_state_p0.switch_u==1)&&(switch_state_p0.switch_prev_u==0))
		{
			if(menu_mode==MENU_DEMO)
			{
				menu_mode=MENU_SOUND;
				strcpy_P(_mes_menu,_mes_menu_select_app_list_1[menu_mode]);
				lcd_put_data(1,_mes_menu);
			}
			else if(menu_mode==MENU_SOUND)
			{
				menu_mode=MENU_DEMO;
				strcpy_P(_mes_menu,_mes_menu_select_app_list_1[menu_mode]);
				lcd_put_data(1,_mes_menu);
			}
		}
		else if((switch_state_p0.switch_d==1)&&(switch_state_p0.switch_prev_d==0))
		{
			if(menu_mode==MENU_DEMO)
			{
				menu_mode=MENU_SOUND;
				strcpy_P(_mes_menu,_mes_menu_select_app_list_1[menu_mode]);
				lcd_put_data(1,_mes_menu);
			}
			else if(menu_mode==MENU_SOUND)
			{
				menu_mode=MENU_DEMO;
				strcpy_P(_mes_menu,_mes_menu_select_app_list_1[menu_mode]);
				lcd_put_data(1,_mes_menu);
			}
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
			menu_exec(menu_mode);
		}
		else if((switch_state_p1.switch_b==1)&&(switch_state_p1.switch_prev_b==0))
		{
		}

		if((switch_state_p1.switch_u==1)&&(switch_state_p1.switch_prev_u==0))
		{
			if(menu_mode==MENU_DEMO)
			{
				menu_mode=MENU_SOUND;
				strcpy_P(_mes_menu,_mes_menu_select_app_list_1[menu_mode]);
				lcd_put_data(1,_mes_menu);
			}
			else if(menu_mode==MENU_SOUND)
			{
				menu_mode=MENU_DEMO;
				strcpy_P(_mes_menu,_mes_menu_select_app_list_1[menu_mode]);
				lcd_put_data(1,_mes_menu);
			}
		}
		else if((switch_state_p1.switch_d==1)&&(switch_state_p1.switch_prev_d==0))
		{
			if(menu_mode==MENU_DEMO)
			{
				menu_mode=MENU_SOUND;
				strcpy_P(_mes_menu,_mes_menu_select_app_list_1[menu_mode]);
				lcd_put_data(1,_mes_menu);
			}
			else if(menu_mode==MENU_SOUND)
			{
				menu_mode=MENU_DEMO;
				strcpy_P(_mes_menu,_mes_menu_select_app_list_1[menu_mode]);
				lcd_put_data(1,_mes_menu);
			}
		}
		else if((switch_state_p1.switch_l==1)&&(switch_state_p1.switch_prev_l==0))
		{
		}
		else if((switch_state_p1.switch_r==1)&&(switch_state_p1.switch_prev_r==0))
		{
		}
	}
}
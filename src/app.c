/*
 * app.c
 *
 * Created: 2012/09/05 22:05:40
 *  Author: makoto
 */ 

/*

	????ファイル??ヘッダ??組??込??アプリケーション??
	「app_"アプリケーション??"」??形??記?q??????????????????。

*/

#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <stdio.h>

#include "app.h"
#include "lcd.h"
#include "led.h"
#include "switch.h"

extern uint8_t _exit_flag;

void app_demo(void)
{	
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

#include "osero.h"

void osero(void){

	map[8][8]=map[7][7]=1;
	map[8][7]=map[7][8]=2;

	map[0][0]=3;

	while(1){
		hyouji();
		idou=0;
		put_check=0;
		//??????????込??
		read_button();

		int hantei=0;
		//自???????褐沚?
		for (map_y = 0; map_y < MAP_SIZE ; map_y++) {
			for (map_x = 0; map_x < MAP_SIZE ; map_x++) {
				if (map[map_y][map_x]==3) {
					map_idou();
					hantei=1;
				}
				if(hantei != 0)
					break;
			}
			if(hantei != 0)
				break;
		}

		iro = (put_kaisuu%2+1);	

	}
}

//????表示
void hyouji(void){
	for (map_y = 0; map_y < MAP_SIZE; map_y++) {		//map??描??
		for (map_x = 0; map_x < MAP_SIZE; map_x++) {
			switch (map[map_y][map_x]){//??
			}
		}
	}
	if (kaisuu==61) {
	}
	sprintf(lcd_bf,"T %2d            ",kaisuu);
	lcd_put_data(0,lcd_bf);

	if (iro==1) {
		lcd_put_data(1,"red             ");
	}else{
		lcd_put_data(1,"green           ");
	}
}

//????????
//??
void map_idou(void){
	switch(idou){
		case 2: {	//??????
							if (map_y+1 < MAP_SIZE) {
								map[map_y][map_x]=old_map;
								led_plot(old_map,map_x,map_y);
								old_map=map[map_y+1][map_x];
								map[map_y+1][map_x]=LED_YELLOW;
								led_plot(LED_YELLOW,map_x+1,map_y);
							}else{
								map[map_y][map_x]=old_map;
								led_plot(old_map,map_x,map_y);
								old_map=map[map_y+1][map_x];
								map[0][map_x]=3;
								led_plot(LED_YELLOW,map_x,0);
							}
						}break;
		case 8:	{	//??????
							if (map_y-1 >= 0) {
								map[map_y][map_x]=old_map;
								led_plot(old_map,map_x,map_y);
								old_map=map[map_y-1][map_x];
								map[map_y-1][map_x]=3;
								led_plot(LED_YELLOW,map_x,map_y-1);
							}else{
								map[map_y][map_x]=old_map;
								led_plot(old_map,map_x,map_y);
								old_map=map[map_y-1][map_x];
								map[MAP_SIZE-1][map_x]=3;
								led_plot(LED_YELLOW,map_x,MAP_SIZE-1);
							}
						}break;
		case 6: {	//右????
							if (map_x+1 < MAP_SIZE) {
								map[map_y][map_x]=old_map;
								led_plot(old_map,map_x,map_y);
								old_map=map[map_y][map_x+1];
								map[map_y][map_x+1]=3;
								led_plot(LED_YELLOW,map_x+1,map_y);
							}else{
								map[map_y][map_x]=old_map;
								led_plot(old_map,map_x,map_y);
								old_map=map[map_y-1][map_x];
								map[map_y][0]=3;
								led_plot(LED_YELLOW,0,map_y);
							}
						}break;
		case 4:{	//??????
						 if (map_x-1 >= 0) {
							 map[map_y][map_x]=old_map;
							 led_plot(old_map,map_x,map_y);
							 old_map=map[map_y][map_x-1];
							 map[map_y][map_x-1]=3;
							 led_plot(LED_YELLOW,map_x-1,map_y);
						 }else{
							 map[map_y][map_x]=old_map;
							 led_plot(old_map,map_x,map_y);	
							 old_map=map[map_y-1][map_x];
							 map[map_y][MAP_SIZE-1]=3;
							 led_plot(LED_YELLOW,MAP_SIZE-1,map_y);
						 }
					 }break;
		case 5:{
						 //??転????
						 map_xx=map_xxx=map_x;
						 map_yy=map_yyy=map_y;
						 if(old_map != 0){
							 break;
						 }
						 //??????	
						 for (; map_yy < MAP_SIZE; map_yy++) {
							 if(map[map_yy+1][map_xx] == 0 || map[map_yy+1][map_xx] == iro || map_yy+2 > MAP_SIZE){
								 break;
							 }
							 if(map[map_yy+2][map_x] == iro){
								 for (; map_yyy < map_yy+2; map_yyy++){
									 map[map_yyy][map_x]=iro;
									 led_plot(iro,map_x,map_yyy);
								 }
								 put_check=1;
							 }
						 }
						 map_xxx=map_xx=map_x;
						 map_yyy=map_yy=map_y;
						 //??????	
						 for (; map_yy > 0 ; map_yy--) {
							 if(map[map_yy-1][map_xx] == 0 || map[map_yy-1][map_xx] == iro || map_yy-2 < 0){
								 break;
							 }
							 if(map[map_yy-2][map_x] == iro){
								 for (; map_yyy > map_yy-2; map_yyy--){
									 map[map_yyy][map_x]=iro;
									 led_plot(iro,map_x,map_yyy);
								 }
								 put_check=1;
							 }
						 }
						 map_xxx=map_xx=map_x;
						 map_yyy=map_yy=map_y;
						 //右????	
						 for (; map_xx < MAP_SIZE; map_xx++) {
							 if(map[map_yy][map_xx+1] == 0 || map[map_yy][map_xx+1] == iro || map_xx+2 > MAP_SIZE){
								 break;
							 }
							 if(map[map_y][map_xx+2] == iro){
								 for (; map_xxx < map_xx+2; map_xxx++){
									 map[map_y][map_xxx]=iro;
									 led_plot(iro,map_x,map_yyy);
								 }
								 put_check=1;
							 }
						 }
						 map_xxx=map_xx=map_x;
						 map_yyy=map_yy=map_y;
						 //??????	
						 for (; map_xx > 0; map_xx--) {
							 if(map[map_yy][map_xx-1] == 0 || map[map_yy][map_xx-1] == iro || map_xx-2 < 0){
								 break;
							 }
							 if(map[map_y][map_xx-2] == iro){
								 for (; map_xxx > map_xx-2; map_xxx--){
									 map[map_y][map_xxx]=iro;
									 led_plot(iro,map_xxx,map_yyy);
								 }
								 put_check=1;
							 }
						 }
						 map_xxx=map_xx=map_x;
						 map_yyy=map_yy=map_y;
						 //右??????	
						 for (; map_xx < MAP_SIZE && map_yy < MAP_SIZE; map_xx++,map_yy++) {
							 if(map[map_yy+1][map_xx+1] == 0 || map[map_yy+1][map_xx+1] == iro || 
									 map_yy+2 > MAP_SIZE || map_xx+2 > MAP_SIZE){
								 break;
							 }
							 if(map[map_yy+2][map_xx+2] == iro){
								 for (; map_xxx < map_xx+2 && map_yyy < map_yy+2; map_xxx++,map_yyy++){
									 map[map_yyy][map_xxx]=iro;
									 led_plot(iro,map_xxx,map_yyy);
								 }
								 put_check=1;
							 }
						 }
						 map_xxx=map_xx=map_x;
						 map_yyy=map_yy=map_y;
						 //????????	
						 for (; map_xx > 0 && map_yy > 0; map_xx--,map_yy--) {
							 if(map[map_yy-1][map_xx-1] == 0 || map[map_yy-1][map_xx-1] == iro ||
									 map_yy-2 < 0 || map_xx-2 < 0){
								 break;
							 }
							 if(map[map_yy-2][map_xx-2] == iro){
								 for (; map_xxx > map_xx-2 && map_yyy > map_yy-2; map_xxx--,map_yyy--){
									 map[map_yyy][map_xxx]=iro;
									 led_plot(iro,map_xxx,map_yyy);
								 }
								 put_check=1;
							 }
						 }
						 map_xxx=map_xx=map_x;
						 map_yyy=map_yy=map_y;
						 //????????	
						 for (; map_xx > 0 && map_yy < MAP_SIZE; map_xx--,map_yy++) {
							 if(map[map_yy+1][map_xx-1] == 0 || map[map_yy+1][map_xx-1] == iro ||
									 map_yy+2 > MAP_SIZE || map_xx-2 < 0){
								 break;
							 }
							 if(map[map_yy+2][map_xx-2] == iro){
								 for (; map_xxx > map_xx-2 && map_yyy < map_yy+2; map_xxx--,map_yyy++){
									 map[map_yyy][map_xxx]=iro;
									 led_plot(iro,map_xxx,map_yyy);
								 }
								 put_check=1;
							 }
						 }
						 map_xxx=map_xx=map_x;
						 map_yyy=map_yy=map_y;
						 //右??????	
						 for (; map_xx < MAP_SIZE && map_yy > 0; map_xx++,map_yy--) {
							 if(map[map_yy-1][map_xx+1] == 0 || map[map_yy-1][map_xx+1] == iro ||
									 map_yy-2 < 0 || map_xx+2 > MAP_SIZE){
								 break;
							 }
							 if(map[map_yy-2][map_xx+2] == iro){ 
								 for (; map_xxx < map_xx+2 && map_yyy > map_yy-2; map_xxx++,map_yyy--){
									 map[map_yyy][map_xxx]=iro;
									 led_plot(iro,map_xxx,map_yyy);
								 }
								 put_check=1;
							 }
						 }
						 if(put_check == 1){
							 //置?????桙?????
							 put_kaisuu++;
							 kaisuu++;
							 //黒・????????
							 map[map_y][map_x] = iro;
							 led_plot(iro,map_x,map_y);
							 old_map = map[map_y][map_x];
							 map[map_y][map_x] = 3;
							 led_plot(iro,map_x,map_y);
						 }
					 }break;
		case 99:{
							put_kaisuu++;
						}break;
		default :{
						 }break;
	}
}
void read_button(void){
	switch_state switch_state_p1;
	switch_state_clear(&switch_state_p1);
	
	if((switch_state_p1.switch_u==1)&&(switch_state_p1.switch_prev_u==0)){
			idou = 8;
		}
		else if((switch_state_p1.switch_d==1)&&(switch_state_p1.switch_prev_d==0)){
			idou = 2;
		}
		else if((switch_state_p1.switch_l==1)&&(switch_state_p1.switch_prev_l==0)){
			idou = 4;
		}
		else if((switch_state_p1.switch_r==1)&&(switch_state_p1.switch_prev_r==0)){
			idou = 6;
		}
		if((switch_state_p1.switch_a==1)&&(switch_state_p1.switch_prev_a==0))
		{
			idou = 5;
		}
		else if((switch_state_p1.switch_b==1)&&(switch_state_p1.switch_prev_b==0))
		{
		}
}

void app_sound(void)
{
	//飽????…

	//switch_state switch_state_p0;
	//switch_state_clear(&switch_state_p0);
	//switch_state switch_state_p1;
	//switch_state_clear(&switch_state_p1);
	//
	//lcd_put_data(0,"Sound Test      ");
	//lcd_put_data(1,"                ");
	//
	//for(;;)
	//{
	//switch_get(SWITCH_CONT_P0,&switch_state_p0);
	//if((switch_state_p0.switch_a==1)&&(switch_state_p0.switch_prev_a==0))
	//{
	//sound_set(SOUND_A,1);
	//}
	//else if((switch_state_p0.switch_b==1)&&(switch_state_p0.switch_prev_b==0))
	//{
	//}
	//
	//if((switch_state_p0.switch_u==1)&&(switch_state_p0.switch_prev_u==0))
	//{
	//}
	//else if((switch_state_p0.switch_d==1)&&(switch_state_p0.switch_prev_d==0))
	//{
	//}
	//else if((switch_state_p0.switch_l==1)&&(switch_state_p0.switch_prev_l==0))
	//{
	//}
	//else if((switch_state_p0.switch_r==1)&&(switch_state_p0.switch_prev_r==0))
	//{
	//}
	//
	//switch_get(SWITCH_CONT_P1,&switch_state_p1);
	//if((switch_state_p1.switch_a==1)&&(switch_state_p1.switch_prev_a==0))
	//{
	//}
	//else if((switch_state_p1.switch_b==1)&&(switch_state_p1.switch_prev_b==0))
	//{
	//}
	//
	//if((switch_state_p1.switch_u==1)&&(switch_state_p1.switch_prev_u==0))
	//{
	//}
	//else if((switch_state_p1.switch_d==1)&&(switch_state_p1.switch_prev_d==0))
	//{
	//}
	//else if((switch_state_p1.switch_l==1)&&(switch_state_p1.switch_prev_l==0))
	//{
	//}
	//else if((switch_state_p1.switch_r==1)&&(switch_state_p1.switch_prev_r==0))
	//{
	//}
	//}
}

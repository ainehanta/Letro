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

#include "osero.h"


void osero_main(void){
	
	map[8][8]=map[7][7]=1;
	led_plot(1,7,7);
	led_plot(1,8,8);
	map[8][7]=map[7][8]=2;
	led_plot(2,7,8);
	led_plot(2,8,7);

	map[0][0]=3;
	led_plot(3,0,0);

	switch_state switch_state_p1;
	switch_state_clear(&switch_state_p1);

	while(1){
		idou=0;
		put_check=0;

		read_button(&switch_state_p1);
		int hantei=0;
		//自???????褐沚?
		map_idou();

		iro = (put_kaisuu%2+1);	
	}
}

//??
//
void map_idou(void){
	switch(idou){
		case 2: {	//??????
							map[map_y][map_x]=old_map;
							led_plot(old_map,map_x,map_y);
							if (map_y+1 < MAP_SIZE) {
								old_map=map[map_y+1][map_x];
								map[map_y+1][map_x]=3;
								led_plot(3,map_x,map_y+1);
								map_y++;
							}else{
								old_map=map[0][map_x];
								map[0][map_x]=3;
								led_plot(3,map_x,0);
								map_y = 0;
							}
						}break;
		case 8:	{	//??????
							map[map_y][map_x]=old_map;
							led_plot(old_map,map_x,map_y);
							if (map_y-1 >= 0) {
								old_map=map[map_y-1][map_x];
								map[map_y-1][map_x]=3;
								led_plot(3,map_x,map_y-1);
								map_y--;
							}else{
								old_map=map[MAP_SIZE-1][map_x];
								map[MAP_SIZE-1][map_x]=3;
								led_plot(3,map_x,MAP_SIZE-1);
								map_y = MAP_SIZE-1; 
							}
						}break;
		case 6: {	//右????
							map[map_y][map_x]=old_map;
							led_plot(old_map,map_x,map_y);
							if (map_x+1 < MAP_SIZE) {
								old_map=map[map_y][map_x+1];
								map[map_y][map_x+1]=3;
								led_plot(3,map_x+1,map_y);
								map_x++;
							}else{
								old_map=map[map_y-1][map_x];
								map[map_y][0]=3;
								led_plot(3,0,map_y);
								map_x=0;
							}
						}break;
		case 4:{	//??????
						 map[map_y][map_x]=old_map;
						 led_plot(old_map,map_x,map_y);	
						 if (map_x-1 >= 0) {
							 old_map=map[map_y][map_x-1];
							 map[map_y][map_x-1]=3;
							 led_plot(3,map_x-1,map_y);
							 map_x--;
						 }else{
							 old_map=map[map_y][MAP_SIZE-1];
							 map[map_y][MAP_SIZE-1]=3;
							 led_plot(3,MAP_SIZE-1,map_y);
							 map_x = MAP_SIZE-1;
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
		case 99:{	put_kaisuu++;	}break;
		default :{ }break;
	}
}

void read_button(switch_state *switch_state_p1){
	int chk;
	chk = 0;
	while(1){
		led_draw_line();
		switch_get(SWITCH_CONT_P1,switch_state_p1);
		if((switch_state_p1->switch_u==1)&&(switch_state_p1->switch_prev_u==0)){
			idou = 8;
			lcd_put_data(1,"ue              ");
			chk = 1;
		}else if((switch_state_p1->switch_d==1)&&(switch_state_p1->switch_prev_d==0)){
			idou = 2;
			lcd_put_data(1,"sita            ");
			chk = 1;
		}else if((switch_state_p1->switch_l==1)&&(switch_state_p1->switch_prev_l==0)){
			idou = 4;
			lcd_put_data(1,"hidari          ");
			chk = 1;
		}else if((switch_state_p1->switch_r==1)&&(switch_state_p1->switch_prev_r==0)){
			idou = 6;
			lcd_put_data(1,"migi            ");
			chk = 1;
		}else if((switch_state_p1->switch_a==1)&&(switch_state_p1->switch_prev_a==0)){
			idou = 5;
			lcd_put_data(1,"put             ");
			chk = 1;
		}else if((switch_state_p1->switch_b==1)&&(switch_state_p1->switch_prev_b==0)){ 
			idou = 99;
			lcd_put_data(1,"skip            ");
			chk = 1;
		}
		if(chk == 1){
			sprintf(lcd_bf,"old_map=%d        ",old_map);
			lcd_put_data(0,lcd_bf);
			break;
		}
	}
}

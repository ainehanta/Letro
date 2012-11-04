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

#define MAP_SIZE 12
static char map[MAP_SIZE][MAP_SIZE]={{0}};
static int map_x,map_y,idou,iro=1;
static int put_kaisuu=0;
static int i,i2;
static int map_xx,map_yy,map_xxx,map_yyy;
static int put_check;
static int old_map;
static int kaisuu=0;
static char lcd_bf[17];

static int red_stone=0, green_stone=0;

static void win(void);

void osero_init(void){
	led_start();

	map[7][7]=map[8][8]=LED_GREEN;
	led_plot(LED_GREEN,7,7);
	led_plot(LED_GREEN,8,8);
	map[7][8]=map[8][7]=LED_RED;
	led_plot(LED_RED,7,8);
	led_plot(LED_RED,8,7);

	map[4][4]=LED_YELLOW;
	led_plot(LED_YELLOW,4,4);

	map_x = map_y = 4;

	//上の線	
	led_plot_line(LED_YELLOW,3,3,12,3);	
	//下の線
	led_plot_line(LED_YELLOW,3,12,12,12);	
	//右の線
	led_plot_line(LED_YELLOW,3,3,3,12);	
	//左の線
	led_plot_line(LED_YELLOW,12,3,12,12);	

	old_map = LED_NONE;

}

void osero_main(void){
	
	switch_state switch_state_p1;
	switch_state_clear(&switch_state_p1);

	switch_state switch_state_p0;
	switch_state_clear(&switch_state_p0);

	osero_init();

	//int hantei=0;
	int blink_led=0;

	while(1){
		idou=0;
		put_check=0;

		_delay_us(1);
		blink_led++;
		
		if(blink_led == 4000){
			led_plot(LED_NONE,map_x,map_y);
			led_plot(old_map,map_x,map_y);
		}
		if(blink_led == 6000){
			led_plot(LED_YELLOW,map_x,map_y);
			blink_led = 0;
		}

		switch_get(SWITCH_CONT_P1,&switch_state_p1);
		if((switch_state_p1.switch_u==1)&&(switch_state_p1.switch_prev_u==0)){
			osero_map_idou_up();
		}else if((switch_state_p1.switch_d==1)&&(switch_state_p1.switch_prev_d==0)){
			osero_map_idou_down();
		}else if((switch_state_p1.switch_l==1)&&(switch_state_p1.switch_prev_l==0)){
			osero_map_idou_left();
		}else if((switch_state_p1.switch_r==1)&&(switch_state_p1.switch_prev_r==0)){
			osero_map_idou_right();
		}else if((switch_state_p1.switch_a==1)&&(switch_state_p1.switch_prev_a==0)){
			osero_map_idou_put();
		}else if((switch_state_p1.switch_b==1)&&(switch_state_p1.switch_prev_b==0)){
			osero_map_idou_skip();
		}

		switch_get(SWITCH_CONT_P0,&switch_state_p0);
		if((switch_state_p0.switch_u==1)&&(switch_state_p0.switch_prev_u==0)){
			osero_map_idou_up();
		}else if((switch_state_p0.switch_d==1)&&(switch_state_p0.switch_prev_d==0)){
			osero_map_idou_down();
		}else if((switch_state_p0.switch_l==1)&&(switch_state_p0.switch_prev_l==0)){
			osero_map_idou_left();
		}else if((switch_state_p0.switch_r==1)&&(switch_state_p0.switch_prev_r==0)){
			osero_map_idou_right();
		}else if((switch_state_p0.switch_a==1)&&(switch_state_p0.switch_prev_a==0)){
			osero_map_idou_put();
		}else if((switch_state_p0.switch_b==1)&&(switch_state_p0.switch_prev_b==0)){
			osero_map_idou_skip();
		}

		iro = (put_kaisuu%2+1);	
	}
}

//マップをクリアー
void osero_map_clear(void){
	int x,y;
	for(y=0;y<16;y++){
		for(x=0;x<16;x++){
			map[y][x] = LED_NONE;
		}
	}
}

//右移動イベントハンドラ
void osero_map_idou_right(void){
	map[map_y][map_x]=old_map;
	led_plot(LED_NONE,map_x,map_y);
	led_plot(old_map,map_x,map_y);
	if (map_x+1 < MAP_SIZE) {
		map_x++;
		old_map=map[map_y][map_x];
		map[map_y][map_x]=LED_YELLOW;
		led_plot(LED_YELLOW,map_x,map_y);
	}else{
		map_x = 4;
		old_map=map[map_y][map_x];
		map[map_y][map_x]=LED_YELLOW;
		led_plot(LED_YELLOW,map_x,map_y);
	}
}

//左移動イベントハンドラ
void osero_map_idou_left(void){
	map[map_y][map_x]=old_map;
	led_plot(LED_NONE,map_x,map_y);
	led_plot(old_map,map_x,map_y);
	if (map_x-1 >= 4) {
		map_x--;
		old_map=map[map_y][map_x];
		map[map_y][map_x]=LED_YELLOW;
		led_plot(LED_YELLOW,map_x,map_y);
	}else{
		map_x = MAP_SIZE-1;
		old_map=map[map_y][map_x];
		map[map_y][map_x]=LED_YELLOW;
		led_plot(LED_YELLOW,map_x,map_y);
	}
}

//上移動イベントハンドラ
void osero_map_idou_up(void){
	map[map_y][map_x]=old_map;
	led_plot(LED_NONE,map_x,map_y);
	led_plot(old_map,map_x,map_y);
	if (map_y-1 >= 4) {
		map_y--;
		old_map=map[map_y][map_x];
		map[map_y][map_x]=LED_YELLOW;
		led_plot(LED_YELLOW,map_x,map_y);
	}else{
		map_y = MAP_SIZE-1;
		old_map=map[map_y][map_x];
		map[map_y][map_x]=3;
		led_plot(LED_YELLOW,map_x,map_y);
	}
}

//下移動イベントハンドラ
void osero_map_idou_down(void){
	map[map_y][map_x]=old_map;
	led_plot(LED_NONE,map_x,map_y);
	led_plot(old_map,map_x,map_y);
	if (map_y+1 < MAP_SIZE) {
		map_y++;
		old_map=map[map_y][map_x];
		map[map_y][map_x]=LED_YELLOW;
		led_plot(LED_YELLOW,map_x,map_y);
	}else{
		map_y = 4;
		old_map=map[map_y][map_x];
		map[map_y][map_x]=3;
		led_plot(LED_YELLOW,map_x,map_y);
	}
}

//プットイベントハンドラ
void osero_map_idou_put(void){
	map_xx=map_xxx=map_x;
	map_yy=map_yyy=map_y;
	if(old_map != 0){
		return;
	}
	//下反転
	for (; map_yy < MAP_SIZE; map_yy++) {
		if(map[map_yy+1][map_xx] == 0 || map[map_yy+1][map_xx] == iro || map_yy+2 > MAP_SIZE){
			break;
		}else if(map[map_yy+2][map_x] == iro){
			for (; map_yyy < map_yy+2; map_yyy++){
				map[map_yyy][map_x]=iro;
				led_plot(LED_NONE,map_x,map_yyy);
				led_plot(iro,map_x,map_yyy);
			}
			put_check=1;
		}
	}
	map_xxx=map_xx=map_x;
	map_yyy=map_yy=map_y;
	//上判定
	for (; map_yy > 4 ; map_yy--) {
		if(map[map_yy-1][map_xx] == 0 || map[map_yy-1][map_xx] == iro || map_yy-2 < 4){
			break;
		}else if(map[map_yy-2][map_x] == iro){
			for (; map_yyy > map_yy-2; map_yyy--){
				map[map_yyy][map_x]=iro;
				led_plot(LED_NONE,map_x,map_yyy);
				led_plot(iro,map_x,map_yyy);
			}
			put_check=1;
		}
	}
	map_xxx=map_xx=map_x;
	map_yyy=map_yy=map_y;
	//右判定
	for (; map_xx < MAP_SIZE; map_xx++) {
		if(map[map_yy][map_xx+1] == 0 || map[map_yy][map_xx+1] == iro || map_xx+2 > MAP_SIZE){
			break;
		}else	if(map[map_y][map_xx+2] == iro){
			for (; map_xxx < map_xx+2; map_xxx++){
				map[map_y][map_xxx]=iro;
				led_plot(LED_NONE,map_xxx,map_y);
				led_plot(iro,map_xxx,map_y);
			}
			put_check=1;
		}
	}
	map_xxx=map_xx=map_x;
	map_yyy=map_yy=map_y;
	//左判定
	for (; map_xx > 0; map_xx--) {
		if(map[map_yy][map_xx-1] == 0 || map[map_yy][map_xx-1] == iro || map_xx-2 < 4){
			break;
		}else if(map[map_y][map_xx-2] == iro){
			for (; map_xxx > map_xx-2; map_xxx--){
				map[map_y][map_xxx]=iro;
				led_plot(LED_NONE,map_xxx,map_yyy);
				led_plot(iro,map_xxx,map_yyy);
			}
			put_check=1;
		}
	}
	map_xxx=map_xx=map_x;
	map_yyy=map_yy=map_y;
	//右上判定
	for (; map_xx < MAP_SIZE && map_yy < MAP_SIZE; map_xx++,map_yy++) {
		if(map[map_yy+1][map_xx+1] == 0 || map[map_yy+1][map_xx+1] == iro ||
				map_yy+2 > MAP_SIZE || map_xx+2 > MAP_SIZE){
			break;
		}else if(map[map_yy+2][map_xx+2] == iro){
			for (; map_xxx < map_xx+2 && map_yyy < map_yy+2; map_xxx++,map_yyy++){
				map[map_yyy][map_xxx]=iro;
				led_plot(LED_NONE,map_xxx,map_yyy);
				led_plot(iro,map_xxx,map_yyy);
			}
			put_check=1;
		}
	}
	map_xxx=map_xx=map_x;
	map_yyy=map_yy=map_y;
	//左下判定
	for (; map_xx > 0 && map_yy > 0; map_xx--,map_yy--) {
		if(map[map_yy-1][map_xx-1] == 0 || map[map_yy-1][map_xx-1] == iro ||
				map_yy-2 < 4 || map_xx-2 < 4){
			break;
		}else if(map[map_yy-2][map_xx-2] == iro){
			for (; map_xxx > map_xx-2 && map_yyy > map_yy-2; map_xxx--,map_yyy--){
				map[map_yyy][map_xxx]=iro;
				led_plot(LED_NONE,map_xxx,map_yyy);
				led_plot(iro,map_xxx,map_yyy);
			}
			put_check=1;
		}
	}
	map_xxx=map_xx=map_x;
	map_yyy=map_yy=map_y;
	//左上判定
	for (; map_xx > 0 && map_yy < MAP_SIZE; map_xx--,map_yy++) {
		if(map[map_yy+1][map_xx-1] == 0 || map[map_yy+1][map_xx-1] == iro ||
				map_yy+2 > MAP_SIZE || map_xx-2 < 4){
			break;
		}else if(map[map_yy+2][map_xx-2] == iro){
			for (; map_xxx > map_xx-2 && map_yyy < map_yy+2; map_xxx--,map_yyy++){
				map[map_yyy][map_xxx]=iro;
				led_plot(LED_NONE,map_xxx,map_yyy);
				led_plot(iro,map_xxx,map_yyy);
			}
			put_check=1;
		}
	}
	map_xxx=map_xx=map_x;
	map_yyy=map_yy=map_y;
	//右上判定
	for (; map_xx < MAP_SIZE && map_yy > 0; map_xx++,map_yy--) {
		if(map[map_yy-1][map_xx+1] == 0 || map[map_yy-1][map_xx+1] == iro ||
				map_yy-2 < 4 || map_xx+2 > MAP_SIZE){
			break;
		}else if(map[map_yy-2][map_xx+2] == iro){
			for (; map_xxx < map_xx+2 && map_yyy > map_yy-2; map_xxx++,map_yyy--){
				map[map_yyy][map_xxx]=iro;
				led_plot(LED_NONE,map_xxx,map_yyy);
				led_plot(iro,map_xxx,map_yyy);
			}
			put_check=1;
		}
	}
	if(put_check == 1){
		//置いた回数
		put_kaisuu++;
		kaisuu++;
		if (put_kaisuu%2) {
		}else{
		}
		if(kaisuu >= 60){
			win();
		}

		map[map_y][map_x] = iro;
		led_plot(LED_NONE,map_x,map_y);
		led_plot(iro,map_x,map_y);
		old_map = map[map_y][map_x];

		map[map_y][map_x] = 3;
		led_plot(LED_NONE,map_x,map_y);
		led_plot(iro,map_x,map_y);
		
		led_plot(LED_YELLOW,map_x,map_y);
		
		//今の数を表示
		for (i = 0; i < 8; i++) {
			for (i2 = 0; i2 < 8; i2++) {
				if(map[i+4][i2+4] == 2){
					red_stone++;
				}else if(map[i+4][i2+4] == 1){
					green_stone++;
				}
			}
		}
		if(kaisuu % 2 == 1){
			green_stone++;
		}else{
			red_stone++;
		}
		sprintf(lcd_bf,"RED   : %2d             ",red_stone);
		lcd_put_data(0,lcd_bf);
		sprintf(lcd_bf,"GREEN : %2d           ",green_stone);
		lcd_put_data(1,lcd_bf);
		
		green_stone = 0;
		red_stone = 0;
		
		led_plot_line(LED_NONE, 0, 0, 0, 15);
		led_plot_line(LED_NONE, 0, 0, 15, 0);
		led_plot_line(LED_NONE, 15, 0, 15, 15);
		led_plot_line(LED_NONE, 0, 15, 15, 15);
		
		led_plot_line(put_kaisuu%2+1, 0, 0, 0, 15);
		led_plot_line(put_kaisuu%2+1, 0, 0, 15, 0);
		led_plot_line(put_kaisuu%2+1, 15, 0, 15, 15);
		led_plot_line(put_kaisuu%2+1, 0, 15, 15, 15);

	}
}

//スキップイベントハンドラ
void osero_map_idou_skip(void){
	put_kaisuu++;
	
	led_plot_line(LED_NONE, 0, 0, 0, 15);
	led_plot_line(LED_NONE, 0, 0, 15, 0);
	led_plot_line(LED_NONE, 15, 0, 15, 15);
	led_plot_line(LED_NONE, 0, 15, 15, 15);

	led_plot_line(put_kaisuu%2+1, 0, 0, 0, 15);
	led_plot_line(put_kaisuu%2+1, 0, 0, 15, 0);
	led_plot_line(put_kaisuu%2+1, 15, 0, 15, 15);
	led_plot_line(put_kaisuu%2+1, 0, 15, 15, 15);

}

//勝敗判定
void win(void){
	int isi_red=0, isi_green=0;

	for(i=0; i<8; i++){
		for(i2=0; i2<8; i2++){
			if(map[i+4][i2+4] == 2){
				lcd_put_data(0,lcd_bf);
				isi_red++;
			}else{
				lcd_put_data(0,lcd_bf);
				isi_green++;
			}
		}
	}
	sprintf(lcd_bf,"RED   : %2d        ",isi_red);
	lcd_put_data(0,lcd_bf);
	sprintf(lcd_bf,"GREEN : %2d     ",isi_green);
	lcd_put_data(1,lcd_bf);

	switch_state switch_state_p1;
	switch_state_clear(&switch_state_p1);

	switch_state switch_state_p0;
	switch_state_clear(&switch_state_p0);


	while(1){
		switch_get(SWITCH_CONT_P1,&switch_state_p1);
		if((switch_state_p1.switch_u==1)&&(switch_state_p1.switch_prev_u==0)){
		}else if((switch_state_p1.switch_d==1)&&(switch_state_p1.switch_prev_d==0)){
			osero_main();
		}else if((switch_state_p1.switch_l==1)&&(switch_state_p1.switch_prev_l==0)){
			osero_main();
		}else if((switch_state_p1.switch_r==1)&&(switch_state_p1.switch_prev_r==0)){
			osero_main();
		}else if((switch_state_p1.switch_a==1)&&(switch_state_p1.switch_prev_a==0)){
			osero_main();
		}else if((switch_state_p1.switch_b==1)&&(switch_state_p1.switch_prev_b==0)){
			osero_main();
		}

		switch_get(SWITCH_CONT_P0,&switch_state_p0);
		if((switch_state_p0.switch_u==1)&&(switch_state_p0.switch_prev_u==0)){
			osero_main();
		}else if((switch_state_p0.switch_d==1)&&(switch_state_p0.switch_prev_d==0)){
			osero_main();
		}else if((switch_state_p0.switch_l==1)&&(switch_state_p0.switch_prev_l==0)){
			osero_main();
		}else if((switch_state_p0.switch_r==1)&&(switch_state_p0.switch_prev_r==0)){
			osero_main();
		}else if((switch_state_p0.switch_a==1)&&(switch_state_p0.switch_prev_a==0)){
			osero_main();
		}else if((switch_state_p0.switch_b==1)&&(switch_state_p0.switch_prev_b==0)){
			osero_main();
		}
	}
}

#undef MAP_SIZE

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

#include "gomoku.h"


void gomoku_init(void){

	lcd_put_data(0,"Green turn      ");

	old_map = LED_NONE;

}

void gomoku_main(void){

	switch_state switch_state_p1;
	switch_state_clear(&switch_state_p1);

	switch_state switch_state_p0;
	switch_state_clear(&switch_state_p0);

	gomoku_init();

	int hantei=0;
	int blink_led=0;

	while(1){
		idou=0;
		put_check=0;

		led_draw_line();
		_delay_us(1);
		blink_led++;

		if(blink_led == 4000){
			led_plot(LED_NONE,map_x,map_y);
		}
		if(blink_led == 5500){
			led_plot(LED_YELLOW,map_x,map_y);
			blink_led = 0;
		}

		switch_get(SWITCH_CONT_P1,&switch_state_p1);
		if((switch_state_p1.switch_u==1)&&(switch_state_p1.switch_prev_u==0)){
			gomoku_map_idou_up();
			lcd_put_data(1,"ue              ");
		}else if((switch_state_p1.switch_d==1)&&(switch_state_p1.switch_prev_d==0)){
			gomoku_map_idou_down();
			lcd_put_data(1,"sita            ");
		}else if((switch_state_p1.switch_l==1)&&(switch_state_p1.switch_prev_l==0)){
			gomoku_map_idou_left();
			lcd_put_data(1,"hidari          ");
		}else if((switch_state_p1.switch_r==1)&&(switch_state_p1.switch_prev_r==0)){
			gomoku_map_idou_right();
			lcd_put_data(1,"migi            ");
		}else if((switch_state_p1.switch_a==1)&&(switch_state_p1.switch_prev_a==0)){
			gomoku_map_idou_put();
			lcd_put_data(1,"put             ");
		}else if((switch_state_p1.switch_b==1)&&(switch_state_p1.switch_prev_b==0)){
			gomoku_map_idou_skip();
			lcd_put_data(1,"skip            ");
		}

		switch_get(SWITCH_CONT_P0,&switch_state_p0);
		if((switch_state_p0.switch_u==1)&&(switch_state_p0.switch_prev_u==0)){
			gomoku_map_idou_up();
			lcd_put_data(1,"ue              ");
		}else if((switch_state_p0.switch_d==1)&&(switch_state_p0.switch_prev_d==0)){
			gomoku_map_idou_down();
			lcd_put_data(1,"sita            ");
		}else if((switch_state_p0.switch_l==1)&&(switch_state_p0.switch_prev_l==0)){
			gomoku_map_idou_left();
			lcd_put_data(1,"hidari          ");
		}else if((switch_state_p0.switch_r==1)&&(switch_state_p0.switch_prev_r==0)){
			gomoku_map_idou_right();
			lcd_put_data(1,"migi            ");
		}else if((switch_state_p0.switch_a==1)&&(switch_state_p0.switch_prev_a==0)){
			gomoku_map_idou_put();
			lcd_put_data(1,"put             ");
		}else if((switch_state_p0.switch_b==1)&&(switch_state_p0.switch_prev_b==0)){
			gomoku_map_idou_skip();
			lcd_put_data(1,"skip            ");
		}

		iro = (put_kaisuu%2+1);	
	}
}

//マップをクリアー
void gomoku_map_clear(void){
	int x,y;
	for(y=0;y<MAP_SIZE;y++){
		for(x=0;x<MAP_SIZE;x++){
			map[y][x] = LED_NONE;
		}
	}
}

//右移動イベントハンドラ
void gomoku_map_idou_right(void){
	map[map_y][map_x]=old_map;
	led_plot(LED_NONE,map_x,map_y);
	led_plot(old_map,map_x,map_y);
	if (map_x+1 < MAP_SIZE) {
		map_x++;
		old_map=map[map_y][map_x];
		map[map_y][map_x]=LED_YELLOW;
		led_plot(LED_YELLOW,map_x,map_y);
	}else{
		map_x = 0;
		old_map=map[map_y][map_x];
		map[map_y][map_x]=LED_YELLOW;
		led_plot(LED_YELLOW,map_x,map_y);
	}
}

//左移動イベントハンドラ
void gomoku_map_idou_left(void){
	map[map_y][map_x]=old_map;
	led_plot(LED_NONE,map_x,map_y);
	led_plot(old_map,map_x,map_y);
	if (map_x-1 >= 0) {
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
void gomoku_map_idou_up(void){
	map[map_y][map_x]=old_map;
	led_plot(LED_NONE,map_x,map_y);
	led_plot(old_map,map_x,map_y);
	if (map_y-1 >= 0) {
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
void gomoku_map_idou_down(void){
	map[map_y][map_x]=old_map;
	led_plot(LED_NONE,map_x,map_y);
	led_plot(old_map,map_x,map_y);
	if (map_y+1 < MAP_SIZE) {
		map_y++;
		old_map=map[map_y][map_x];
		map[map_y][map_x]=LED_YELLOW;
		led_plot(LED_YELLOW,map_x,map_y);
	}else{
		map_y = 0;
		old_map=map[map_y][map_x];
		map[map_y][map_x]=3;
		led_plot(LED_YELLOW,map_x,map_y);
	}
}

//プットイベントハンドラ
void gomoku_map_idou_put(void){
  
  roop=0;					//x軸についての評価
  while(roop<=4){
    xhyouka=1;	
    xi=0;
    xx = map_x-roop;
    while(xi<=4){				//5個のかたまり	
      //printf("masu[%3d][%3d]=%d ",xx+xi,ya,masu[xx+xi][ya]);
      xhyouka *= map[map_y][map_x+xi];
      xi++;
    }
    //printf("xhyouka=%d",xhyouka);
    if (xhyouka==1)
      //printf("白の勝ち");
    if (xhyouka==32)
      //printf("黒の勝ち");
      //puts("");
    roop++;
  }
  roop=0;						//y軸についての評価
  while(roop<=4){
    yhyouka=1;
    yi=0;
    yy = map_y-roop;
    while(yi<=4){
      //printf("masu[%3d][%3d]=%d ",xa,yy+yi,masu[xa][yy+yi]);
      yhyouka *= map[map_y+yi][map_x];
      yi++;
    }
    //printf("yhyouka=%d",yhyouka);
    if (yhyouka==1)
      //printf("白の勝ち");
    if (yhyouka==32)
      //printf("黒の勝ち");
      //puts("");
    roop++;
  }
  roop=0;						//斜めの評価
  while(roop<=4){
    naname=1;
    na=0;
    xna = map_x-roop;
    yna = map_y-roop;
    while(na<=4){
      //printf("masu[%3d][%3d]=%d ",xna+na,yna+na,masu[xna+na][yna+na]);
      naname *= map[yna+na][xna+na];
      na++;
    }
    //printf("naname=%d",naname);
    if (naname==1)
      //printf("白の勝ち");
    if (naname==32)
      //printf("黒の勝ち");
      //puts("");
    roop++;
  }
  roop=0;
  while(roop<=4){
    naname2=1;
    na2=0;
    xna2 = map_x-roop;
    yna2 = map_y+roop;
    while(na2<=4){
      //printf("masu[%3d][%3d]=%d ",xna2-na2,yna2-na2,masu[xna2+na2][yna2-na2]);
      naname2 *= map[yna2+na2][xna2-na2];
      na2++;
    }
    //printf("naname2=%d",naname2);
    if (naname2==1)
      //printf("白の勝ち");
    if (naname2==32)
      //printf("黒の勝ち");
      //puts("");
    roop++;
  }
  
}

//スキップイベントハンドラ
void gomoku_map_idou_skip(void){
	put_kaisuu++;
	if (put_kaisuu%2) {
		lcd_put_data(0,"Red turn        ");
	}else{
		lcd_put_data(0,"Green turn      ");
	}
}

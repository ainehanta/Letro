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

#define MAP_SIZE 16

void gomoku_init(void){

	int x=0;
	int y=0;
	for(y=0;y<MAP_SIZE;y++)
	{
		for(x=0;x<MAP_SIZE;x++)
		{
			map[y][x] = 0;
		}
	}

	iro = 1;
	put_kaisuu = 0;
	old_map = 0;
	kaisuu = 1;

	lcd_put_data(0,"Green turn      ");
	led_start();

	gomoku_map_clear();
	
	win=0;

	led_plot(LED_YELLOW,0,0);
	map[0][0]=LED_YELLOW;

	map_x=0;
	map_y=0;
	
	iro=1;
	check=0;
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
	  map_check();
	  map_draw();
	  if(iro==1&&win==0){
	    lcd_put_data(0,"Green turn      ");
	  }else if(win==0){
	    lcd_put_data(0,"Red   turn      ");
	  }
	  idou=0;
	  put_check=0;
	  
	  //led_draw_line();
	  _delay_us(1);
	  blink_led++;
	  
	  if(win==1){
	    lcd_put_data(1,"thx 4 playing :)");
	  }

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
	  }else if((switch_state_p1.switch_d==1)&&(switch_state_p1.switch_prev_d==0)){
	    gomoku_map_idou_down();
	  }else if((switch_state_p1.switch_l==1)&&(switch_state_p1.switch_prev_l==0)){
	    gomoku_map_idou_left();
	  }else if((switch_state_p1.switch_r==1)&&(switch_state_p1.switch_prev_r==0)){
	    gomoku_map_idou_right();
	  }else if((switch_state_p1.switch_a==1)&&(switch_state_p1.switch_prev_a==0)){
	    if(win==0){
	      gomoku_map_idou_put();
	    }else{
	      /*
	      lcd_put_data(0,"Thank you for   ");
	      lcd_put_data(1,"      Playing!! ");
	      _delay_us(1000);
	      lcd_put_data(0,"Letro: SelectApp");
	      return ;
	    */
	    }
	  }else if((switch_state_p1.switch_b==1)&&(switch_state_p1.switch_prev_b==0)){
	    //gomoku_map_idou_skip();
	    //lcd_put_data(1,"skip            ");
	  }
	    map_check();
	  switch_get(SWITCH_CONT_P0,&switch_state_p0);
	  if((switch_state_p0.switch_u==1)&&(switch_state_p0.switch_prev_u==0)){
	    gomoku_map_idou_up();
	    if(win==0){
	    }
	  }else if((switch_state_p0.switch_d==1)&&(switch_state_p0.switch_prev_d==0)){
	    gomoku_map_idou_down();
	    if(win==0){
	    }
	  }else if((switch_state_p0.switch_l==1)&&(switch_state_p0.switch_prev_l==0)){
	    gomoku_map_idou_left();
	    if(win==0){
	    }
	  }else if((switch_state_p0.switch_r==1)&&(switch_state_p0.switch_prev_r==0)){
	    gomoku_map_idou_right();
	    if(win==0){
	    }
	  }else if((win==0&&switch_state_p0.switch_a==1)&&(switch_state_p0.switch_prev_a==0)){
	    if(win==0){
	      gomoku_map_idou_put();
	    }
	  }else if((switch_state_p0.switch_b==1)&&(switch_state_p0.switch_prev_b==0)){
	    //gomoku_map_idou_skip();
	    //	lcd_put_data(1,"skip            ");
	  }
	  iro = (put_kaisuu%2+1);
	}
}
	//マップをクリアー
void gomoku_map_clear(void){
  int x,y;
  for(y=0;y<MAP_SIZE;y++){
    for(x=0;x<MAP_SIZE;x++){
      map[y][x] = 0;
    }
  }
}

//右移動イベントハンドラ
void gomoku_map_idou_right(void){
  map[map_x][map_y]=old_map;
  led_plot(LED_NONE,map_x,map_y);
  led_plot(old_map,map_x,map_y);
  if (map_x+1 < MAP_SIZE) {
    map_x++;
    old_map=map[map_x][map_y];
    map[map_x][map_y]=LED_YELLOW;
    led_plot(LED_YELLOW,map_x,map_y);
  }else{
    map_x = 0;
    old_map=map[map_x][map_y];
    map[map_x][map_y]=LED_YELLOW;
    led_plot(LED_YELLOW,map_x,map_y);
  }
}

//左移動イベントハンドラ
void gomoku_map_idou_left(void){
  map[map_x][map_y]=old_map;
  led_plot(LED_NONE,map_x,map_y);
  led_plot(old_map,map_x,map_y);
  if (map_x-1 >= 0) {
    map_x--;
    old_map=map[map_x][map_y];
    map[map_x][map_y]=LED_YELLOW;
    led_plot(LED_YELLOW,map_x,map_y);
  }else{
    map_x = MAP_SIZE-1;
    old_map=map[map_x][map_y];
    map[map_x][map_y]=LED_YELLOW;
    led_plot(LED_YELLOW,map_x,map_y);
  }
}

//上移動イベントハンドラ
void gomoku_map_idou_up(void){
  map[map_x][map_y]=old_map;
  led_plot(LED_NONE,map_x,map_y);
  led_plot(old_map,map_x,map_y);
  if (map_y-1 >= 0) {
    map_y--;
    old_map=map[map_x][map_y];
    map[map_x][map_y]=LED_YELLOW;
    led_plot(LED_YELLOW,map_x,map_y);
  }else{
    map_y = MAP_SIZE-1;
    old_map=map[map_x][map_y];
     map[map_x][map_y]=3;
    led_plot(LED_YELLOW,map_x,map_y);
  }
}

void map_draw(void){
  for(check_i=0;check_i<MAP_SIZE;check_i++){
    for(check_j=0;check_j<MAP_SIZE;check_j++){
      led_plot(map[check_i][check_j],check_i,check_j);
    }
  }
}

//下移動イベントハンドラ
void gomoku_map_idou_down(void){
  map[map_x][map_y]=old_map;
  led_plot(LED_NONE,map_x,map_y);
  led_plot(old_map,map_x,map_y);
  if (map_y+1 < MAP_SIZE) {
    map_y++;
    old_map=map[map_x][map_y];
    map[map_x][map_y]=LED_YELLOW;
    led_plot(LED_YELLOW,map_x,map_y);
  }else{
    map_y = 0;
    old_map=map[map_x][map_y];
    map[map_x][map_y]=3;
    led_plot(LED_YELLOW,map_x,map_y);
  }
}

//プットイベントハンドラ
void gomoku_map_idou_put(void){
  if(check!=0){
    if(old_map==0){
      map[map_x][map_y]=iro;
      map_check();
      old_map=iro;
      map[map_x][map_y]=3;
      if(iro==2){
	//led_plot(LED_RED,map_x,map_y);
      }else{
	//led_plot(LED_GREEN,map_x,map_y);
      }
      put_kaisuu++;
    }
  }else{
    check=1;
  }
  return ;
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

void map_check(void){
  //MAP_SIZE
  
  //縦
  for(check_j=0;check_j<MAP_SIZE;check_j++){
    for(check_i=0;check_i<MAP_SIZE-4;check_i++){
      if(map[check_j][check_i] == 1 && map[check_j][check_i+1] == 1 && map[check_j][check_i+2] == 1 && map[check_j][check_i+3] == 1 && map[check_j][check_i+4] == 1){
	win=1;
	lcd_put_data(0,"Green win       ");
	lcd_put_data(1,"thx 4 playing :)");
      }
      if(map[check_j][check_i] == 2 && map[check_j][check_i+1] == 2 && map[check_j][check_i+2] == 2 && map[check_j][check_i+3] == 2 && map[check_j][check_i+4] == 2){
	win=1;
	lcd_put_data(0,"Red   win       ");
	lcd_put_data(1,"thx 4 playing :)");
      }
    }
  }
  


  //横

  for(check_j=0;check_j<MAP_SIZE;check_j++){
    for(check_i=0;check_i<MAP_SIZE-4;check_i++){
      if(map[check_i][check_j] == 1 && map[check_i+1][check_j] == 1 && map[check_i+2][check_j] == 1 && map[check_i+3][check_j] == 1 && map[check_i+4][check_j] == 1){
	  win=1;
	  lcd_put_data(0,"Green win       ");
	  lcd_put_data(1,"thx 4 playing :)");
	}
      if(map[check_i][check_j] == 2 && map[check_i+1][check_j] == 2 && map[check_i+2][check_j] == 2 && map[check_i+3][check_j] == 2 && map[check_i+4][check_j] == 2){
	 win=1;
	 lcd_put_data(0,"Red   win       ");
	 lcd_put_data(1,"thx 4 playing :)");
      }
    }
  }
  

  
  //斜め 右下がり
  
  for(check_j=0;check_j<MAP_SIZE-4;check_j++){
    for(check_i=0;check_i<MAP_SIZE-4;check_i++){
      if(map[check_j][check_i] == 1 && map[check_j+1][check_i+1] == 1 && map[check_j+2][check_i+2] == 1 && map[check_j+3][check_i+3] == 1 && map[check_j+4][check_i+4] == 1){
	win=1;
	lcd_put_data(0,"Green win       ");
	lcd_put_data(1,"thx 4 playing :)");
      }
      if(map[check_j][check_i] == 2 && map[check_j+1][check_i+1] == 2 && map[check_j+2][check_i+2] == 2 && map[check_j+3][check_i+3] == 2 && map[check_j+4][check_i+4] == 2){
	win=1;
	lcd_put_data(0,"Red   win       ");
	lcd_put_data(1,"thx 4 playing :)");
      }
    }
  }


  //斜め 左下がり
  
  for(check_j=MAP_SIZE-1;check_j>4;check_j--){
    for(check_i=0;check_i<MAP_SIZE-4;check_i++){
      if(map[check_j][check_i] == 1 && map[check_j-1][check_i+1] == 1 && map[check_j-2][check_i+2] == 1 && map[check_j-3][check_i+3] == 1 && map[check_j-4][check_i+4] == 1){
	win=1;
	lcd_put_data(0,"Green win       ");
	lcd_put_data(1,"thx 4 playing:) ");
      }
      if(map[check_j][check_i] == 2 && map[check_j-1][check_i+1] == 2 && map[check_j-2][check_i+2] == 2 && map[check_j-3][check_i+3] == 2 && map[check_j-4][check_i+4] == 2){
	win=1;
	lcd_put_data(0,"Red   win       ");
	lcd_put_data(1,"thx 4 playing :)");
      }
    }
  }

}
#undef MAP_SIZE

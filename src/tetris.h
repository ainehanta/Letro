#if !defined(HAS_CURSES)
# define ENABLE_AVR
#endif

#if defined HAS_CURSES && defined ENABLE_AVR
# error AVRとHAS_CURSESが両方共defineされています
#endif

#include <stdlib.h>
#include <stdio.h>

//AVR
#if defined ENABLE_AVR
#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#ifndef _BUILD_
	#include <avr/iom1284p.h>
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "app.h"
#include "lcd.h"
#include "led.h"
#include "switch.h"
#endif

//PC用
#if defined HAS_CURSES
#define LED_NONE			0
#define	LED_GREEN			1
#define LED_RED				2
#define LED_YELLOW			3


#include <signal.h>
#include <ncurses.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>

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
#endif

typedef struct{
 unsigned int LED_height;
 unsigned int LED_width;
 unsigned int LCD_height;
 unsigned int LCD_width;
}Tetris_Hard_Info;

typedef struct{
 unsigned int block_down_time;
 unsigned int map_x;
 unsigned int map_y;
}Tetris_Data_List;

typedef struct{
 int block_x;
 int block_y;
 int next_block_no;
 int using_block[4][4];

 unsigned int deletedLine;
 unsigned int score;

 int generateFlag;
 int endFlag;
 int flame;
 int map[16][10];
}Tetris_Data;

typedef struct{
 Tetris_Hard_Info *hard;
 Tetris_Data_List *tetris;
 Tetris_Data *data;
 switch_state sw;
}TetrisWorld;

int call_tetris(void);

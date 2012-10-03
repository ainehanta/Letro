#ifndef	OSERO_ 
#define OSERO_

#define MAP_SIZE 16
void hyouji(void);
void map_idou(void);
void read(void);

char map[MAP_SIZE][MAP_SIZE]={0};
int map_x,map_y,idou,iro=1;
int put_kaisuu=0;
int i,i2;
int map_xx,map_yy,map_xxx,map_yyy;
int put_check;
int old_map=0;
int kaisuu=1;
char lcd_bf[50];

void read_button(void);
void hyouji(void);
void map_idou(void);
void read_button(void);
void osero(void);

#endif /* SWITCH_H_ */


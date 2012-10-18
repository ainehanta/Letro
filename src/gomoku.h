#ifndef	GOMOKU_ 
#define GOMOKU_

#define MAP_SIZE 16

char map[MAP_SIZE][MAP_SIZE]={0};
int map_x,map_y,idou,iro=1;
int put_kaisuu=0;
int i,i2;
int map_xx,map_yy,map_xxx,map_yyy;
int put_check;
int old_map=0;
int kaisuu=1;

int roop;
int xhyouka;	
int xi;
int xx;
int yhyouka;
int yi;
int yy;
int naname;
int na;
int xna,yna;
int naname2;
int na2;
int xna2,yna2;

int result;

char lcd_bf[17];

void map_idou(void);
void read_button(switch_state*);

//メイン及び初期化関数
void gomoku_main(void);
void gomoku_init(void);
void gomoku_map_clear(void);

//移動用のイベントハンドラ
void gomoku_map_idou_right(void);
void gomoku_map_idou_left(void);
void gomoku_map_idou_up(void);
void gomoku_map_idou_down(void);
void gomoku_map_idou_put(void);
void gomoku_map_idou_skip(void);

#endif

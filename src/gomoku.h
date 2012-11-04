#ifndef	GOMOKU_ 
#define GOMOKU_

#define MAP_SIZE 16

char map[MAP_SIZE][MAP_SIZE];
int map_x,map_y,idou,iro;
int put_kaisuu;
int i,i2;
int map_xx,map_yy,map_xxx,map_yyy;
int put_check;
int old_map;
int kaisuu;

int check;

int win;
int check_i;
int check_j;
int check_k;

int result;

char lcd_bf[17];

void map_check(void);
void map_draw(void);

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

#undef MAP_SIZE
#endif

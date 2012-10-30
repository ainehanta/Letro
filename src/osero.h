#ifndef	OSERO_ 
#define OSERO_

#define MAP_SIZE 16

extern char map[MAP_SIZE][MAP_SIZE];
extern int map_x;
extern int map_y;
extern int idou;
extern int iro;
extern int put_kaisuu;
int i,i2;
int map_xx,map_yy,map_xxx,map_yyy;
int put_check;
extern int old_map;
extern int kaisuu;
char lcd_bf[17];

void map_idou(void);
void read_button(switch_state*);

//メイン及び初期化関数
void osero_main(void);
void osero_init(void);
void osero_map_clear(void);

//移動用のイベントハンドラ
void osero_map_idou_right(void);
void osero_map_idou_left(void);
void osero_map_idou_up(void);
void osero_map_idou_down(void);
void osero_map_idou_put(void);
void osero_map_idou_skip(void);

#endif /* SWITCH_H_ */

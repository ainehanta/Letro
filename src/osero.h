#ifndef	OSERO_ 
#define OSERO_


#define MAP_SIZE 12

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

#undef MAP_SIZE

#endif /* SWITCH_H_ */

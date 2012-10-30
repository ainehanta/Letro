#include "tetris.h"

#if defined(HAS_CURSES)
WINDOW *LCD;
WINDOW *realLCD;
WINDOW *LED;
WINDOW *fakeLED;
#endif


//初期化系
static void initialize(TetrisWorld *thisData);
static void setHardStatus(Tetris_Hard_Info *thisHard);
static void setTetrisData(TetrisWorld *thisData);
static void initData(TetrisWorld *thisData);
#if defined(HAS_CURSES)
static void setNcurses(TetrisWorld *thisData);
static void signalhandler(int sig);
#elif defined(ENABLE_AVR)
static void initAVR(void);
#endif

//TCNT0

static void loop(TetrisWorld *thisData);
static void draw(TetrisWorld *thisData);
static void set_block(int block_no,int block[4][4]);
static void generate_block(TetrisWorld *thisData);
static void move_block(TetrisWorld *thisData);
static int conflict(TetrisWorld *thisData);
static int gameover(TetrisWorld *thisData);
static void set_map_block(TetrisWorld *thisData);
static void deleteLine(TetrisWorld *thisData);
static void rotateblock(TetrisWorld *thisData,int mode);
static void drawNextBlock(TetrisWorld *thisData);

static void free_this(TetrisWorld *thisData);

//ハード依存系
static void my_led_plot(uint8_t color,uint8_t y,uint8_t x);
static void my_lcd_write(uint8_t row,const char* str);
static void my_set_switch(switch_state *target);

#if defined(HAS_CURSES)
int main(void){
 call_tetris();
 return 0;
}
#endif

static void free_this(TetrisWorld *thisData){
 free(thisData->hard);
 free(thisData->tetris);
 free(thisData->data);
 free(thisData);

}

int call_tetris(void){
 TetrisWorld *this=(TetrisWorld *)malloc(sizeof(TetrisWorld));
 this->hard=(Tetris_Hard_Info *)malloc(sizeof(Tetris_Hard_Info));
 this->tetris=(Tetris_Data_List *)malloc(sizeof(Tetris_Data_List));
 this->data=(Tetris_Data *)malloc(sizeof(Tetris_Data));
 initialize(this);
 while(!this->data->endFlag){
  this->data->flame++;
  loop(this);
#if defined(HAS_CURSES)
  wrefresh(LED);
  wrefresh(realLCD);
  usleep((1000/60)*1000);
#elif defined(ENABLE_AVR)
  _delay_us((1000/60)*1000);
#endif
 }
#if defined(HAS_CURSES)
 free_this(this);
 endwin();
#endif

 return 0;
}

void initialize(TetrisWorld *thisData){
 int i;

 setHardStatus(thisData->hard);
#if defined HAS_CURSES
 srand((unsigned)time(NULL));
 setNcurses(thisData);
#elif defined ENABLE_AVR
 srand(TCNT0);
 initAVR();
#endif
 setTetrisData(thisData);
 initData(thisData);
 //描画
 for(i=0;i<(int)thisData->hard->LED_height;i++){
  my_led_plot(LED_YELLOW,i,10);
 }
 //LCD初期化
 my_lcd_write(0,"                    ");
 my_lcd_write(1,"                    ");
 deleteLine(thisData);
}

static void setHardStatus(Tetris_Hard_Info *thisHard){
 thisHard->LED_height=16;
 thisHard->LED_width=16;
 thisHard->LCD_height=2;
 thisHard->LCD_width=16;
}

#if defined(ENABLE_AVR)
void initAVR(void){
 led_start();
}
#endif
#if defined HAS_CURSES
void setNcurses(TetrisWorld *thisData){
 if(signal(SIGINT,signalhandler)==SIG_ERR){
  fprintf(stderr,"SIGNALHANDLER ERROR\n");
  exit(EXIT_FAILURE);
 }

 initscr();
 cbreak();
 noecho();
 nodelay(stdscr,TRUE);
 keypad(stdscr,TRUE);
 start_color();
 use_default_colors();
 init_pair(1,-1,COLOR_GREEN);
 init_pair(2,-1,COLOR_RED);
 init_pair(3,-1,COLOR_YELLOW);

 fakeLED=newwin(thisData->hard->LED_height+2,thisData->hard->LED_width+2,5,2);
 LED=subwin(fakeLED,thisData->hard->LED_height,thisData->hard->LED_width,6,3);
 LCD=newwin(thisData->hard->LCD_height+2,thisData->hard->LCD_width+2,0,0);
 realLCD=subwin(LCD,thisData->hard->LCD_height,thisData->hard->LCD_width,1,1);
 if(LED==NULL||LCD==NULL||realLCD==NULL||fakeLED==NULL){
  endwin();
  if(LED==NULL){
   fprintf(stderr,"LED IS NULL\n");
  }
  if(fakeLED==NULL){
   fprintf(stderr,"fakeLED IS NULL\n");
  }
  exit(EXIT_FAILURE);
 }

 //LED
 keypad(LED,TRUE);
 nodelay(LED,TRUE);

 box(LCD,'|','-');
 box(fakeLED,'|','-');
 wrefresh(LCD);
 wrefresh(fakeLED);

}
#endif

void setTetrisData(TetrisWorld *thisData){
 thisData->tetris->block_down_time=60;
 thisData->tetris->map_y=16;
 thisData->tetris->map_x=10;
}

void initData(TetrisWorld *thisData){
 int i,j;
 //thisData->data->next_block_no=rand()%7;
 thisData->data->generateFlag=1;
 thisData->data->endFlag=0;
 thisData->data->flame=0;

 thisData->data->deletedLine=0;
 thisData->data->score=0;

 for(i=0;i<16;i++){
  for(j=0;j<10;j++){
   thisData->data->map[i][j]=0;
  }
 }
}

void set_block(int block_no,int block[4][4]){
 static unsigned int bList[7][4][4]={
  {{0,0,0,0},
   {0,1,1,0},
   {0,1,1,0},
   {0,0,0,0}},
  {{0,1,0,0},
   {0,1,0,0},
   {0,1,0,0},
   {0,1,0,0}},
  {{0,1,1,0},
   {0,1,0,0},
   {0,1,0,0},
   {0,0,0,0}},
  {{0,1,1,0},
   {0,0,1,0},
   {0,0,1,0},
   {0,0,0,0}},
  {{0,0,1,0},
   {0,1,1,0},
   {0,1,0,0},
   {0,0,0,0}},
  {{0,1,0,0},
   {0,1,1,0},
   {0,0,1,0},
   {0,0,0,0}},
  {{0,0,0,0},
   {0,0,1,0},
   {0,1,1,1},
   {0,0,0,0}}
 };

 int i,j;
 
 for(i=0;i<4;i++){
  for(j=0;j<4;j++){
   block[i][j]=bList[block_no][i][j];
  }
 }
}

void loop(TetrisWorld *thisData){
 if(thisData->data->generateFlag){
  generate_block(thisData);
 }else{
  move_block(thisData);
 }
 draw(thisData);
}

void move_block(TetrisWorld *thisData){
 //入力
 int tmp;
 int move_y;
 int move_x=0;
 int rotate;
 switch_state *sw=&(thisData->sw);

 my_set_switch(sw);
 //連打補正
 if(sw->switch_a&&(!sw->switch_prev_a)){
  rotate=1;
 }else if(sw->switch_b&&(!sw->switch_prev_b)){
  rotate=-1;
 }
 if(sw->switch_l){
  move_x=-1;
 }else if(sw->switch_r){
  move_x=1;
 }


 if(rotate!=0){
  rotateblock(thisData,rotate);
  tmp=conflict(thisData);
  if(tmp){
   rotateblock(thisData,-rotate);
  }
 }

 if(sw->switch_d||thisData->data->flame%thisData->tetris->block_down_time==0){
  move_y=1;
  thisData->data->flame=0;
 }else{
  move_y=0;
 }

 thisData->data->block_x+=move_x;
 tmp=conflict(thisData);
 if(tmp){
  thisData->data->block_x-=move_x;
  move_x=0;
 }
 if(move_y){
  thisData->data->block_y+=move_y;
  tmp=conflict(thisData);
  if(tmp){
   thisData->data->block_y-=1;
   if(!move_x){
    set_map_block(thisData);
    deleteLine(thisData);
    thisData->data->generateFlag=1;
   }
  }
 }
}

#if defined HAS_CURSES
void signalhandler(int sig){
 endwin();
 fprintf(stderr,"SIG:%d\n",sig);
 sig=sig;
 exit(EXIT_FAILURE);
}
#endif

void generate_block(TetrisWorld *thisData){
 thisData->data->flame=0;
 thisData->data->generateFlag=0;
 thisData->data->block_x=3;
 thisData->data->block_y=0;
 set_block(thisData->data->next_block_no,thisData->data->using_block);

 thisData->data->next_block_no=rand()%7;
 drawNextBlock(thisData);

 if(conflict(thisData)){
  gameover(thisData);
 }
}

int conflict(TetrisWorld *thisData){
 int i,j;
 int tmp;
 int res=0;

 for(i=0;i<4;i++){
  for(j=0;j<4;j++){
   tmp=(
     (thisData->data->block_y+i<0)||
     (thisData->data->block_y+i>(int)thisData->tetris->map_y-1)||
     (thisData->data->block_x+j<0)||
     (thisData->data->block_x+j>(int)thisData->tetris->map_x-1)
     );
   if(!tmp){
    tmp|=(thisData->data->map[thisData->data->block_y+i][thisData->data->block_x+j]?2:0);
   }
   tmp=(tmp&&thisData->data->using_block[i][j]);
   res|=tmp;
  }
 }
 return res;
}

void draw(TetrisWorld *thisData){
 int i,j;

 for(i=0;i<(int)thisData->tetris->map_y;i++){
  for(j=0;j<(int)thisData->tetris->map_x;j++){
   switch(thisData->data->map[i][j]){
    case 0:
     my_led_plot(LED_NONE,i,j);
     break;
    case 1:
     my_led_plot(LED_RED,i,j);
     break;
    default:
#if defined HAS_CURSES
     waddch(LED,'?');
#endif
     break;
   }
  }
 }

 for(i=0;i<4;i++){
  for(j=0;j<4;j++){
   if(thisData->data->block_y+i<0||
      thisData->data->block_x+j<0||
      thisData->data->block_y+i>(int)thisData->tetris->map_y-1||
      thisData->data->block_x+j>(int)thisData->tetris->map_x-1){
    continue;
   }
   if(thisData->data->using_block[i][j]){
    my_led_plot(LED_GREEN,thisData->data->block_y+i,thisData->data->block_x+j);
   }
  }
 }
}

void drawNextBlock(TetrisWorld *thisData){
 int dummy[4][4];
 int i,j;

 set_block(thisData->data->next_block_no,dummy);
 for(i=0;i<4;i++){
  for(j=0;j<4;j++){
   if(dummy[i][j]){
    my_led_plot(LED_RED,1+i,11+j);
   }else{
    my_led_plot(LED_NONE,1+i,11+j);
   }
  }
 }
}


void deleteLine(TetrisWorld *thisData){
#define MAP_Y thisData->tetris->map_y
#define MAP_X thisData->tetris->map_x
#define MAP thisData->data->map
 int i,j,k;
 int flag;
 int dLine=0;
 char wData[21];

 for(i=0;i<(int)MAP_Y;i++){
  flag=MAP[MAP_Y-1-i][0];
  for(j=1;j<(int)MAP_X;j++){
   flag&=MAP[MAP_Y-1-i][j];
  }
  if(flag){
   dLine++;
   for(k=i;k<(int)MAP_Y-1;k++){
    for(j=0;j<(int)MAP_X;j++){
     MAP[MAP_Y-1-k][j]=MAP[MAP_Y-1-k-1][j];
    }
   }
   for(j=0;j<(int)MAP_X-1;j++){
    MAP[0][j]=0;
   }
   i--;
  }
 }
 thisData->data->deletedLine+=dLine;
 switch (dLine) {
  case 1:
   thisData->data->score+=100;
   break;
  case 2:
   thisData->data->score+=250;
   break;
  case 3:
   thisData->data->score+=500;
  case 4:
   thisData->data->score+=1000;
  default:
   break;
 }
 snprintf(wData,17,"DELETEDLINE:%4d",thisData->data->deletedLine);
 my_lcd_write(0,wData);
 snprintf(wData,17,"SCORE:%10d",thisData->data->score);
 my_lcd_write(1,wData);
#undef MAP_Y
#undef MAP_X
#undef MAP
}

void rotateblock(TetrisWorld *thisData,int mode){
#define BLOCK thisData->data->using_block
 int i,j;
 int tmp[4][4];

 for(i=0;i<4;i++){
  for(j=0;j<4;j++){
   if(mode<0){
    tmp[i][j]=BLOCK[j][3-i];
   }else if(mode>0){
    tmp[i][j]=BLOCK[3-j][i];
   }
  }
 }

 for(i=0;i<4;i++){
  for(j=0;j<4;j++){
   BLOCK[i][j]=tmp[i][j];
  }
 }
#undef BLOCK
}

//ブロック固定
void set_map_block(TetrisWorld *thisData){
#define BLOCK_X thisData->data->block_x
#define BLOCK_Y thisData->data->block_y
#define BLOCK thisData->data->using_block
#define MAP thisData->data->map
 int i,j;
 for(i=0;i<4;i++){
  for(j=0;j<4;j++){
   MAP[BLOCK_Y+i][BLOCK_X+j]|=BLOCK[i][j];
  }
 }
#undef BLOCK_X
#undef BLOCK_Y
#undef BLOCK
#undef MAP
}

int gameover(TetrisWorld *thisData){
 int flag=0;

 draw(thisData);
 switch_state *sw=&(thisData->sw);
// flag=flag;
 my_lcd_write(0,"GAME OVER           ");
 my_lcd_write(1,"PRESS A BUTTON      ");
 //スコア表示
 while(!flag){
  my_set_switch(sw);
  if(sw->switch_a){
   flag=1;
  }
  _delay_us((1000/60)*1000);
 }
 my_lcd_write(0,"A:GO BACK MENU  ");
 my_lcd_write(1,"B:RESTART TETRIS");
 while(flag){
  my_set_switch(sw);
  if(sw->switch_a||sw->switch_b){
   flag=0;
  }
  _delay_us((1000/60)*1000);
 }
 if(sw->switch_a){
  thisData->data->endFlag=1;
 }else if(sw->switch_b){
  initialize(thisData);
 }
 return 0;
}

void my_led_plot(uint8_t color,uint8_t y,uint8_t x){
#if defined(HAS_CURSES)
 mvwaddch(LED,y,x,' '|COLOR_PAIR(color));
#elif defined(ENABLE_AVR)
 led_plot(color,y,x);
#endif
}
void my_lcd_write(uint8_t row,const char* str){
#if defined(HAS_CURSES)
 int i;

 for(i=0;*str!='\0'&&i<16;str++,i++){
  mvwaddch(realLCD,row,i,*str|COLOR_PAIR(0));
 }
 wrefresh(realLCD);
#elif defined(ENABLE_AVR)
 lcd_put_data(row,str);
#endif
}

static void my_set_switch(switch_state *target){
#if defined(HAS_CURSES)
 int ch;

 ch=wgetch(LED);
 target->switch_prev_a = target->switch_a;
 target->switch_prev_b = target->switch_b;
 target->switch_prev_u = target->switch_u;
 target->switch_prev_d = target->switch_d;
 target->switch_prev_l = target->switch_l;
 target->switch_prev_r = target->switch_r;
 target->switch_a=0;
 target->switch_b=0;
 target->switch_u=0;
 target->switch_d=0;
 target->switch_l=0;
 target->switch_r=0;

 switch(ch){
  case KEY_LEFT:
  case 'h':
   target->switch_l=1;
   break;
  case KEY_RIGHT:
  case 'l':
   target->switch_r=1;
   break;
  case KEY_DOWN:
  case 'j':
   target->switch_d=1;
   break;
  case KEY_UP:
  case 'k':
  case ' ':
   target->switch_u=1;
   break;
  case 'x':
   target->switch_a=1;
   break;
  case 'z':
   target->switch_b=1;
   break;
  default:
   break;
 }

#elif defined(ENABLE_AVR)
 switch_state_clear(target);
#endif
}

/*
 * lcd.h
 *
 * Created: 2012/09/05 13:25:04
 *  Author: makoto
 */ 


#ifndef LCD_H_
#define LCD_H_


//LCD用通信ピン
//必ず一連のポート番号0～6を指定してください。
#define DB0		PORTC0
#define DB1		PORTC1
#define DB2		PORTC2
#define DB3		PORTC3
#define RS		PORTC4
#define R_W		PORTC5
#define E		PORTC6

#define LCD_PINS _BV(DB0) | _BV(DB1) | _BV(DB2) | _BV(DB3) | _BV(E) | _BV(RS) | _BV(R_W)
#define LCD_PORT PORTC
#define LCD_PIN	 PINC
#define LCD_DDR	 DDRC

void _init_4bit(void);
void _bf_check(void);
void _func_set(void);
void _on_off_ctrl(uint8_t);
void _display_clear(void);
void _ent_mode_set(void);
void _lcd_ddram_adr_set(uint8_t);
void _lcd_putchar(const char);
void lcd_print(const char*);
uint8_t _my_strcat(char*,const char*);
void lcd_init(void);
void lcd_put_data(uint8_t,const char*);


#endif /* LCD_H_ */
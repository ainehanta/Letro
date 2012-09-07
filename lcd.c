/*
 * lcd.c
 *
 * Created: 2012/09/05 13:24:43
 *  Author: makoto
 */ 

#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

#include "lcd.h"

uint8_t _lcd_cursor;

//LCDモジュールを4bit modeにする（BF使用不可）
//絶対に直接使用しないこと
//最悪LCDとの通信ができなくなります
void _init_4bit(void)
{
	_delay_ms(15);

	LCD_PORT = _BV(E) | 0b0011;
	LCD_PORT = LCD_PORT & ~(_BV(E));

	_delay_ms(4.1);

	LCD_PORT = _BV(E) | 0b0011;
	LCD_PORT = LCD_PORT & ~(_BV(E));

	_delay_us(100);

	LCD_PORT = _BV(E) | 0b0011;
	LCD_PORT = LCD_PORT & ~(_BV(E));

	_delay_ms(10);

	LCD_PORT = _BV(E) | 0b0001;
	LCD_PORT = LCD_PORT & ~(_BV(E));

	_delay_ms(10);
}

//ビジーフラッグのチェック
//何かLCDに対する低レベルな処理を行う関数を作成した場合使用してください。
//直接使用は禁止
//というか意味が無い
void _bf_check(void)
{
	uint8_t data = 0;
	uint16_t count;
	DDRC &= ~(_BV(DB0)|_BV(DB1)|_BV(DB2)|_BV(DB3));

	for(count=0;;count++)
	{
		LCD_PORT = _BV(R_W) | _BV(E);
		data = (PINC & 0x0F)<<4;
		LCD_PORT &= ~(_BV(R_W) | _BV(E));
		
		LCD_PORT = _BV(R_W) | _BV(E);
		data |= (PINC & 0x0F);
		LCD_PORT &= ~(_BV(R_W) | _BV(E));
		
		if((data>>7)&1==0||count>=512)
			break;
			
		_delay_us(1);
	}
	DDRC |= LCD_PINS;
}

//ファンクションセット
//絶対に直接実行してはいけない
//最悪LCDとの通信ができなくなります
void _func_set(void)
{
	LCD_PORT = _BV(E) | 0b0010;
	LCD_PORT = LCD_PORT & ~(_BV(E));
	LCD_PORT = _BV(E) | 0b1000;
	LCD_PORT = LCD_PORT & ~(_BV(E));

	_bf_check();
}

//LCDの表示設定
//使用しても構わないが手動での使用は非推奨
void _on_off_ctrl(uint8_t data)
{
	LCD_PORT = _BV(E);
	LCD_PORT = LCD_PORT & ~(_BV(E));
	LCD_PORT = _BV(E) | _BV(DB3) | data;
	LCD_PORT = LCD_PORT & ~(_BV(E));

	_bf_check();
}

//LCDをクリア
//使用しても構わないが手動での使用は非推奨
void _display_clear(void)
{
	LCD_PORT = _BV(E);
	LCD_PORT = LCD_PORT & ~(_BV(E));
	LCD_PORT = _BV(E) | 0b0001;
	LCD_PORT = LCD_PORT & ~(_BV(E));

	_bf_check();
}

//エントリーモードのセット
//てか直接使用しないでください
void _ent_mode_set(void)
{
	LCD_PORT = _BV(E);
	LCD_PORT = LCD_PORT & ~(_BV(E));
	LCD_PORT = _BV(E) | 0b0110;
	LCD_PORT = LCD_PORT & ~(_BV(E));

	_bf_check();
}

//LCDのカーソルを指定したアドレスに移動
//使用しても構わないが表示できる範囲と_lcd_cursorの整合を取ること
//なるべく使用する場合はアクセス用の関数を書いて_lcd_cursorの整合を取ること
//addr : 移動したいアドレス
void _lcd_ddram_addr_set(uint8_t addr)
{
	LCD_PORT = _BV(E) | _BV(DB3) | ((addr>>4)&0x0F);
	LCD_PORT = LCD_PORT & ~(_BV(E));
	LCD_PORT = _BV(E) | (addr&0x0F);
	LCD_PORT = LCD_PORT & ~(_BV(E));
	
	_bf_check();
}

//LCDに一文字出力
//前処理なしで出力するので危険
//直接使用しないでください
//ch : 書き込む文字
void _lcd_putchar(const char ch)
{
	LCD_PORT = _BV(RS) | _BV(E) | ((ch>>4)&0x0F);
	LCD_PORT = LCD_PORT & ~(_BV(E));
	LCD_PORT = _BV(RS) | _BV(E) | (ch&0x0F);
	LCD_PORT = LCD_PORT & ~(_BV(E));
	_lcd_cursor++;
	
	_bf_check();
}

//LCDに文字列を出力
//前処理なしに出力するので危険
//直接使用しないでください
//str : 書き込む文字列
void _lcd_print(const char *str)
{
	uint8_t i;

	for(i=0;str[i]!='\0';i++)
		_lcd_putchar(str[i]);
}

//文字列の結合
//結合された文字数が欲しいので作成
//直接使用しないでください
//str1 : 結合先
//str2 : 結合元
//return : 結合した文字数
uint8_t _my_strcat(char* str1,const char* str2)
{
	uint8_t x,y;

	for(x=0;str1[x]!='\0';x++);

	for(y=0;str2[y]!='\0';y++)
	{
		str1[x] = str2[y];
	}
	str1[x] = '\0';

	return y;
}

//LCDの初期化
void lcd_init(void)
{
	LCD_DDR |= LCD_PINS;
	_init_4bit();
	_func_set();
	_on_off_ctrl(0b100);
	_display_clear();
	_ent_mode_set();
	_lcd_cursor = 0x00;
}

//LCDに実際にデータを書き込む
//row : LCDの行
//str : 書き込む文字列
void lcd_put_data(uint8_t row,const char* str)
{
	uint8_t i;
	if(row==0)
	{
		_lcd_cursor = 0x00;
	}
	else if(row==1)
	{
		_lcd_cursor = 0x40;
	}
	_lcd_ddram_addr_set(_lcd_cursor);
	for(i=0;(str[i]!='\0')&&(i<16);i++)
	{
		_lcd_putchar(str[i]);
	}
}
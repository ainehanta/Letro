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

//LCD���W���[����4bit mode�ɂ���iBF�g�p�s�j
//��΂ɒ��ڎg�p���Ȃ�����
//�ň�LCD�Ƃ̒ʐM���ł��Ȃ��Ȃ�܂�
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

//�r�W�[�t���b�O�̃`�F�b�N
//����LCD�ɑ΂���჌�x���ȏ������s���֐����쐬�����ꍇ�g�p���Ă��������B
//���ڎg�p�͋֎~
//�Ƃ������Ӗ�������
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

//�t�@���N�V�����Z�b�g
//��΂ɒ��ڎ��s���Ă͂����Ȃ�
//�ň�LCD�Ƃ̒ʐM���ł��Ȃ��Ȃ�܂�
void _func_set(void)
{
	LCD_PORT = _BV(E) | 0b0010;
	LCD_PORT = LCD_PORT & ~(_BV(E));
	LCD_PORT = _BV(E) | 0b1000;
	LCD_PORT = LCD_PORT & ~(_BV(E));

	_bf_check();
}

//LCD�̕\���ݒ�
//�g�p���Ă��\��Ȃ����蓮�ł̎g�p�͔񐄏�
void _on_off_ctrl(uint8_t data)
{
	LCD_PORT = _BV(E);
	LCD_PORT = LCD_PORT & ~(_BV(E));
	LCD_PORT = _BV(E) | _BV(DB3) | data;
	LCD_PORT = LCD_PORT & ~(_BV(E));

	_bf_check();
}

//LCD���N���A
//�g�p���Ă��\��Ȃ����蓮�ł̎g�p�͔񐄏�
void _display_clear(void)
{
	LCD_PORT = _BV(E);
	LCD_PORT = LCD_PORT & ~(_BV(E));
	LCD_PORT = _BV(E) | 0b0001;
	LCD_PORT = LCD_PORT & ~(_BV(E));

	_bf_check();
}

//�G���g���[���[�h�̃Z�b�g
//�Ă����ڎg�p���Ȃ��ł�������
void _ent_mode_set(void)
{
	LCD_PORT = _BV(E);
	LCD_PORT = LCD_PORT & ~(_BV(E));
	LCD_PORT = _BV(E) | 0b0110;
	LCD_PORT = LCD_PORT & ~(_BV(E));

	_bf_check();
}

//LCD�̃J�[�\�����w�肵���A�h���X�Ɉړ�
//�g�p���Ă��\��Ȃ����\���ł���͈͂�_lcd_cursor�̐�������邱��
//�Ȃ�ׂ��g�p����ꍇ�̓A�N�Z�X�p�̊֐���������_lcd_cursor�̐�������邱��
//addr : �ړ��������A�h���X
void _lcd_ddram_addr_set(uint8_t addr)
{
	LCD_PORT = _BV(E) | _BV(DB3) | ((addr>>4)&0x0F);
	LCD_PORT = LCD_PORT & ~(_BV(E));
	LCD_PORT = _BV(E) | (addr&0x0F);
	LCD_PORT = LCD_PORT & ~(_BV(E));
	
	_bf_check();
}

//LCD�Ɉꕶ���o��
//�O�����Ȃ��ŏo�͂���̂Ŋ댯
//���ڎg�p���Ȃ��ł�������
//ch : �������ޕ���
void _lcd_putchar(const char ch)
{
	LCD_PORT = _BV(RS) | _BV(E) | ((ch>>4)&0x0F);
	LCD_PORT = LCD_PORT & ~(_BV(E));
	LCD_PORT = _BV(RS) | _BV(E) | (ch&0x0F);
	LCD_PORT = LCD_PORT & ~(_BV(E));
	_lcd_cursor++;
	
	_bf_check();
}

//LCD�ɕ�������o��
//�O�����Ȃ��ɏo�͂���̂Ŋ댯
//���ڎg�p���Ȃ��ł�������
//str : �������ޕ�����
void _lcd_print(const char *str)
{
	uint8_t i;

	for(i=0;str[i]!='\0';i++)
		_lcd_putchar(str[i]);
}

//������̌���
//�������ꂽ���������~�����̂ō쐬
//���ڎg�p���Ȃ��ł�������
//str1 : ������
//str2 : ������
//return : ��������������
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

//LCD�̏�����
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

//LCD�Ɏ��ۂɃf�[�^����������
//row : LCD�̍s
//str : �������ޕ�����
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
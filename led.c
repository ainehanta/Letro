/*
 * led.c
 *
 * Created: 2012/09/05 13:40:08
 *  Author: makoto
 */ 

#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

#include "led.h"

uint16_t *led_fb;
uint16_t led_fb01[LED_WIDTH*2];
uint16_t anode_data;
uint16_t kasode_data[2];
uint8_t row_count;

void led_init(void)
{
	SHIFT_DDR |= SHIFT_PINS;
	led_fb = led_fb01;
	led_clear();
	led_draw_full();
}

//�V���A���N���b�N���M
void _shift_sck(void)
{
	SHIFT_PORT &= ~_BV(SHIFT_SCK);
	SHIFT_PORT |= _BV(SHIFT_SCK);
}

//���b�`�N���b�N���M
void _shift_rck(void)
{
	SHIFT_PORT &= ~_BV(SHIFT_RCK);
	SHIFT_PORT |= _BV(SHIFT_RCK);
}

//�V�t�g���W�X�^�Ƀr�b�g�𑗐M����B
//bit : ���肽���r�b�g�i�o�C�g�ł͂Ȃ��̂Œ���!�j
void _shift_bit_out(uint8_t bit)
{
	if(bit)
	{
		SHIFT_PORT |= _BV(SHIFT_DATA);
	}
	else
	{
		SHIFT_PORT &= ~_BV(SHIFT_DATA);
	}
}

//�V�t�g���W�X�^�Ƀf�[�^���M
//�f�[�^�������瑗�M
void _shift_data_out(uint16_t data)
{
	uint8_t i;

	for(i=0;i<16;i++)
	{
		_shift_bit_out((data>>i)&1);
		_shift_sck();
	}
}

//�A�m�[�h���V�t�g���W�X�^�p�̑��M�f�[�^���쐬�B
//row : �s�̎w��
uint16_t _mk_anode_data(uint8_t row)
{
	uint16_t ret = 0b1000000000000000;

	if(row>=0 && row<16)
	{
		ret >>= row;
	}

	return ret;
}

//�J�\�[�h���V�t�g���W�X�^�p�̑��M�f�[�^���t���[���o�b�t�@�p�z�񂩂�쐬�B
//row : �s�̎w��
void _mk_kasode_data(uint16_t* ret,uint8_t row)
{
	if(row>=0 && row<16)
	{
		ret[0] = ~led_fb[row];
		ret[1] = ~led_fb[row+16];
	}
}

void led_draw_line(void)
{
	if(row_count>=16)
	{
		anode_data = 0b1000000000000000;
		row_count = 0;
	}

	_mk_kasode_data(kasode_data,row_count);

	_shift_data_out(kasode_data[1]);

	_shift_data_out(kasode_data[0]);

	_shift_data_out(anode_data);

	_shift_rck();

	anode_data >>= 1;
	row_count++;
}

//�t���[���o�b�t�@�̓��e���h�b�g�}�g���N�XLED�ɕ`��
void led_draw_full(void)
{
	uint8_t i;

	//�t���[���o�b�t�@�̍s�������[�v
	for(i=0;i<LED_WIDTH;i++)
	{
		led_draw_line();
	}
	
	_shift_data_out(0);
	_shift_data_out(0);
	_shift_data_out(0);
	_shift_data_out(0);
	_shift_rck();
}

//LED��S����
//��{�I�Ƀ��[�h�؂�ւ����ɂ̂ݎg�p���邱��
void led_clear(void)
{
	uint8_t i;

	anode_data = 0b1000000000000000;
	row_count = 0;

	for(i=0;i<LED_WIDTH*2;i++)
	{
		led_fb[i] = 0;
	}
}

void led_plot(uint8_t color,uint8_t y,uint8_t x)
{
	if(color==LED_NONE)
	{
		if(x<16&&y<16)
		{
			led_fb[x] &= ~(0b1000000000000000>>y);
			led_fb[x+16] &= ~(0b1000000000000000>>y);
		}
	}
	else if(color==LED_GREEN)
	{
		if(x<16&&y<16)
		{
			led_fb[x] |= (0b1000000000000000>>y);
		}
	}
	else if(color==LED_RED)
	{
		if(x<16&&y<16)
		{
			led_fb[x+16] |= (0b1000000000000000>>y);
		}
	}
	else if(color==LED_YELLOW)
	{
		if(x<16&&y<16)
		{
			led_fb[x] |= (0b1000000000000000>>y);
			led_fb[x+16] |= (0b1000000000000000>>y);
		}
	}
}

void led_plot_line(uint8_t color,int8_t x1,int8_t y1 ,int8_t x2 ,int8_t y2)
{
	int8_t xd;
	int8_t yd;
	int8_t xs=1;
	int8_t ys=1;
	int8_t e;

	xd = x2 - x1;
	if(xd < 0){
		xd = -xd;
		xs = -1;
	}
	yd = y2 - y1;
	if(yd < 0){
		yd = -yd;
		ys = -1;
	}
	led_plot(color,x1,y1);
	e = 0;
	if( yd < xd ) {
		while( x1 != x2) {
			x1 += xs;
			e += (2 * yd);
			if(e >= xd) {
				y1 += ys;
				e -= (2 * xd);
			}
			led_plot(color,x1,y1);
		}
	}else{
		while( y1 != y2) {
			y1 += ys;
			e += (2 * xd);
			if(e >= yd) {
				x1 += xs;
				e -= (2 * yd);
			}
			led_plot(color,x1,y1);
		}
	}
}

void led_scroll(int8_t x,int8_t y)
{
	uint8_t i;

	if(x!=0)
	{
		if(x>0)
		{
			for(i=0;i<LED_WIDTH;i++)
			{
				led_fb[i] <<= x;
			}
		}
		else
		{
			for(i=0;i<LED_WIDTH;i++)
			{
				led_fb[i] >>= x;
			}
		}
	}

	if(x!=0)
	{
		if(y>0)
		{
			for(i=0;i<LED_WIDTH;i++)
			{
				if((i+y)>15)
				led_fb[i] = 0;
				else
				led_fb[i] = led_fb[i+y];
			}
		}
		else
		{
			for(i=LED_WIDTH-1;i>=0;i--)
			{
				if((i+y)<0)
				led_fb[i] = 0;
				else
				led_fb[i] = led_fb[i+y];
			}
		}
	}
}

uint16_t* _led_get_fb_addr(void)
{
	return led_fb;
}

void _led_set_fb_addr(uint16_t *addr)
{
	led_fb = addr;
}
/*
 * led.h
 *
 * Created: 2012/09/05 13:40:19
 *  Author: makoto
 */ 


#ifndef LED_H_
#define LED_H_


#define SHIFT_SCK			PORTB0
#define SHIFT_RCK			PORTB1
#define SHIFT_DATA			PORTB2

#define SHIFT_PINS			_BV(SHIFT_SCK) | _BV(SHIFT_RCK) | _BV(SHIFT_DATA)
#define SHIFT_PORT			PORTB
#define SHIFT_PIN			PINB
#define SHIFT_DDR			DDRB

#define LED_WIDTH			16

#define LED_NONE			0
#define	LED_GREEN			1
#define LED_RED				2
#define LED_YELLOW			3

void led_init(void);
void _shift_sck(void);
void _shift_rck(void);
void _shift_bit_out(uint8_t);
void _shift_data_out(uint16_t);
uint16_t _mk_anode_data(uint8_t);
void _mk_kasode_data(uint16_t*,uint8_t);
void led_draw_line(void);
void led_draw_full(void);
void led_clear(void);
void led_plot(uint8_t,uint8_t,uint8_t);
void led_plot_line(uint8_t,int8_t,int8_t,int8_t,int8_t);
void led_scroll(int8_t,int8_t);
uint16_t* led_get_fb_addr(void);
void led_set_fb_addr(uint16_t*);


#endif /* LED_H_ */
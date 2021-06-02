#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "TM4C123.h"
#include "delay_timer.h"
#include <stdint.h>
#include <string.h>

#define OUTPUT 0xFF
#define INPUT	 0x00

#define clearLower 0xF0  /* used in LCD_nibble_write function */
#define clearUpper 0x0F

#define PIN_0	0x01	//bit mask pin0 or GPIOA
#define PIN_1 0x02	//bit mask pin1 or GPIOB
#define PIN_2 0x04	//bit mask pin2 or GPIOC
#define PIN_3 0x08	//bit mask pin3 or GPIOD
#define PIN_4 0x10	//bit mask pin4 or GPIOE
#define PIN_5 0x20	//bit mask pin5 or GPIOF
#define PIN_6 0x40
#define PIN_7 0x80

#define LCD_PORT GPIOB
#define RS PIN_0		//pin0 portB
#define RW PIN_1		//pin1 portB
#define EN PIN_2		//pin2 portB

void LCD_nibble_write(unsigned char data, unsigned char control);
void LCD_command(unsigned char command);
void LCD_data(unsigned char data);
void LCD_init(void);

#endif
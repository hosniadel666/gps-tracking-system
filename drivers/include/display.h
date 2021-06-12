#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <stdint.h>
#include "TM4C123.h"
#include "delay_timer.h"

// LCD dimensions
#define LCD_WIDTH 20
#define LCD_HEIGHT 4

#define CLEAN_CMD 0x01

// GPIO pins used for data port
/*
	LCD		|	DB4	  |	|	DB4	  ||	DB4	  ||	DB4	  |
				 ^			 ^			 ^ 			 ^
	TM4C123	|	PD0	  |	|	PD1	  ||	PD2	  ||	PD3	  |
*/
#define DB4_PIN 0
#define DB5_PIN 1
#define DB6_PIN 2
#define DB7_PIN 3

// GPIO pins used for control port
/*
	LCD		|	RS	  |	|	RW	  ||	EN	  |
				 ^			 ^			 ^ 			 
	TM4C123	|	PE1	  |	|	PE2	  ||	PE3	  |
*/
#define RS_PIN	1
#define RW_PIN	2
#define EN_PIN	3

// Initialization settings
#define BLINK_ON		1		// 0: cursor blink is off			1: cursor blink is on		
#define CURSOR_ON		0		// 0: cursor is off					2: cursor is on
#define	DISPLAY_ON		4		// 0: display is off				4: display is on

void lcd_init(void);
void lcd_send_Nibble(uint8_t );
void lcd_set_cursor(uint8_t , uint8_t );
void lcd_write_command(uint8_t );
void lcd_write_character(uint8_t );
void lcd_print(char *, uint32_t );
void lcd_clean();

#endif // __DISPLAY__
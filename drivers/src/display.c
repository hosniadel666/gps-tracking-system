#include "display.h"

// Mask of used pins
#define DB4 (1U << DB4_PIN)
#define DB5 (1U << DB5_PIN)
#define DB6 (1U << DB6_PIN)
#define DB7 (1U << DB7_PIN)
#define RS	(1U << RS_PIN)
#define RW  (1U << RW_PIN)
#define EN  (1U << EN_PIN)

void lcd_send_Nibble(uint8_t n) 
{
	// Format the byte to use the correct bits in the data register
	uint8_t fn = 0;	
	fn |= (n & 0x01) << DB4_PIN;
	fn |= ((n & 0x02) >> 1) << DB5_PIN;
	fn |= ((n & 0x04) >> 2) << DB6_PIN;
	fn |= ((n & 0x08) >> 3) << DB7_PIN;
	// Send the byte to the LCD 
	GPIOD->DATA	&= ~(DB4 | DB5 | DB6 | DB7);
	GPIOD->DATA |= fn;
	GPIOE->DATA |= EN;
	GPIOE->DATA &= ~EN;
}


void lcd_write_command(uint8_t c) {
	// Set control pins to write, 
	GPIOE->DATA &= ~(RS | RW | EN);
	lcd_send_Nibble(c  >> 4);
	lcd_send_Nibble(c & 0x0F);

	delay_lcd(7000);
}


void lcd_write_character(uint8_t c) 
{
	GPIOE->DATA &= ~(RS | RW | EN);
	GPIOE->DATA |= RS;
	lcd_send_Nibble(c >> 4);
	lcd_send_Nibble(c & 0x0F);

	delay_lcd(4000);
	
}

void lcd_print(char *s, uint32_t length) {
	uint32_t i;
	for(i = 0; i < length; i++) {
		lcd_write_character(s[i]);
	}
}

void lcd_set_cursor(uint8_t row, uint8_t column) {
	uint8_t ddram_addr = row % 2 == 0 ? 0x00 : 0x40;
	ddram_addr += row >= (LCD_HEIGHT / 2) ? LCD_WIDTH : 0;
	ddram_addr += column;
	lcd_write_command(0x80 | ddram_addr);
}

void lcd_init() 
{
	SYSCTL->RCGCGPIO |= 0x08;						// Enable clock for data GPIODs port
	GPIOD->DIR |= DB4 | DB5 | DB6 | DB7;			// Set pins to output
	GPIOD->DEN |= DB4 | DB5 | DB6 | DB7;			// Digital Enable
	GPIOD->AMSEL &= ~(DB4 | DB5 | DB6 | DB7);		// Disable analog function
	GPIOD->PDR |= DB4 | DB5 | DB6 | DB7;
	
	SYSCTL->RCGCGPIO |= 0x10;						// Enable clock for GPIOE port
	GPIOE->DIR |= RS | RW | EN;						// Set pins to output
	GPIOE->DEN |= RS | RW | EN;						// Digital Enable
	GPIOE->AMSEL &= ~(RS | RW | EN);				// Disable analog function
	
	delay_lcd(100000);										// Initial delay to allow LCD to fully power up

	lcd_write_command(0x28);									 // Configure to 4-bit data, 2 display lines, 5x8 dot font
	lcd_write_command(0x08 | DISPLAY_ON | CURSOR_ON | BLINK_ON); // Set display, cursor, and cursor blink bits
		
	lcd_write_command(0x01);								// Clear display
	lcd_write_command(0x06);								// Configure to cursor increment, no shift
}

void lcd_clean()
{
	lcd_write_command(CLEAN_CMD);
}


#include "display.h"

void LCD_nibble_write(unsigned char data, unsigned char control)
{
	  data &= clearLower;       /* clear lower nibble for control */
    control &= clearUpper;    /* clear upper nibble for data */
    LCD_PORT->DATA = data | control;        /* RS = 0, R/W = 0 */
	  LCD_PORT->DATA = data | control | EN;   /* pulse E */
    delay_micros(1);
    LCD_PORT->DATA = data;
    LCD_PORT->DATA = 0;
}

void LCD_command(unsigned char command)
{
		LCD_nibble_write(command & clearLower, 0);  /* upper nibble first */
    LCD_nibble_write(command << 4, 0);      /* then lower nibble */
   
    if (command < 4)
    delay_ms(2);       /* commands 1 and 2 need up to 1.64ms */
	else
    delay_micros(40); 
}

void LCD_data(unsigned char data)
{
	  LCD_nibble_write(data & clearLower, RS);   /* upper nibble first */
    LCD_nibble_write(data << 4, RS);     /* then lower nibble */
 
    delay_micros(40);
}

void LCD_init(void)
{
	  SYSCTL->RCGCGPIO |= PIN_1;      /* enable clock to GPIOB */
    LCD_PORT->DIR = OUTPUT;         /* set all PORTB pins as output */
    LCD_PORT->DEN = OUTPUT;         /* set all PORTB pins as digital pins */
    delay_ms(20);
	
		/* initialization sequence */
    LCD_nibble_write(0x30, 0);
    delay_ms(5);
    LCD_nibble_write(0x30, 0);
    delay_micros(100);
    LCD_nibble_write(0x30, 0);
    delay_micros(40);
    LCD_nibble_write(0x20, 0);  /* use 4-bit data mode */
    delay_micros(50);
		
	  LCD_command(0x02); 	        /* return home - 4 bit mode only */
    LCD_command(0x28);          /* set 4-bit mode */
    LCD_command(0x06);          /* shift cursor right */
    LCD_command(0x01);          /* clear screen */
    LCD_command(0x0F);          /* turn on display, cursor blinking */
		LCD_command(0x80);					/* cursor to beginning of 1st line */
}

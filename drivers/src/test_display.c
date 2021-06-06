#include "stdint.h"
#include "tm4c123gh6pm.h"

	
void lcd_command(unsigned char command){
		GPIO_PORTA_DATA_R = 0;
		GPIO_PORTB_DATA_R = command;
		
		GPIO_PORTA_DATA_R |= 0x80;
		delay_ms(230);
		GPIO_PORTA_DATA_R = 0;
}


void lcd_data(char data){
		GPIO_PORTA_DATA_R = 0x20;
		GPIO_PORTB_DATA_R = data;
		
		GPIO_PORTA_DATA_R |= 0x80;
		delay_ms(230);
		GPIO_PORTA_DATA_R = 0;

}
void init(){
	uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x03; // portA for ctrl , portB for data
	delay=1; // Dummy variable 

	GPIO_PORTB_DIR_R = 0xFF; // for data
	GPIO_PORTB_DEN_R= 0xFF;
	
	GPIO_PORTA_DIR_R = 0xE0; // last 3 bits
	GPIO_PORTA_DEN_R= 0xE0;
	
	lcd_command(0x30);
	lcd_command(0x38);
	lcd_command(0x06);
	lcd_command(0x01);
	lcd_command(0x0F);
}
	
	
int test_main(){

	init();
	while(1){
		lcd_command(0x01);
		lcd_command(0x80);
		
		lcd_data('A');
		lcd_data('B');
	}
}
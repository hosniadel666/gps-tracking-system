#include "led.h"

void led_init(led_t led)
{
	SYSCTL->RCGCGPIO |= 0X20;		
	GPIOF->DIR |= led;
	GPIOF->DEN |= led;
	GPIOF->DATA = 0;
}

void led_on(led_t led)
{
	GPIOF->DATA |= led; 
}

void led_off(led_t led)
{
	GPIOF->DATA &= ~led; 
}

void all_on()
{
	SYSCTL->RCGCGPIO |= 0x20;	
	GPIOF->DIR |= 0x0E;
	GPIOF->DEN |= 0x0E;
	GPIOF->DATA |= 0x0E; 
}

void all_off()
{
	SYSCTL->RCGCGPIO |= 0x20;		
	GPIOF->DIR |= 0x0E;
	GPIOF->DEN |= 0x0E;
	GPIOF->DATA &= ~0x0E; 
}
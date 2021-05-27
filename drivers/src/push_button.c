#include "push_button.h"

void push_button_init(push_button_t button)
{
	SYSCTL->RCGCGPIO |= 0X20;
	if(SW2 == button)
	{
		GPIOF->LOCK = 0x4C4F434B;   // UnlockGPIOCR register because PF0 is locked pin 
		GPIOF->CR = 0x01;           // Make the corresponding GPIOAFSEL, GPIOPUR, GPIOPDR, or GPIODEN bits can be written
	}

	GPIOF->DIR &= ~button;
	GPIOF->DEN |= button;
	GPIOF->PUR |= button;
}

int push_button_status(push_button_t button)
{
	return (int)GPIOF->DATA & button;
}
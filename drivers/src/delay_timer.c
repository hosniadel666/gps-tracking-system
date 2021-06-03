#include "delay_timer.h"


void delay_micros(uint32_t n)
{
	SYSCTL->RCGCTIMER |= 0x08; 	// enable clock to Timer Block 3
	TIMER3->CTL = 0;			// disable Timer before initialization 
	TIMER3->CFG = 0x04;			// 16-bit option 
	TIMER3->TBMR = 0x02; 		// periodic mode and down-counter 
	TIMER3->TBILR = 16 - 1; 	// TimerB interval load value reg 
	TIMER3->ICR = 0x100; 		// clear the TimerB timeout flag
	TIMER3->CTL |= 0x0100; 		// enable TimerB after initialization 
	
	for(uint32_t i = 0; i < n; i++)
	{
		while ((TIMER3->RIS & 0x100) == 0);  	// wait for TimerB timeout flag 1 us 
		TIMER3->ICR = 0x100; 					// clear the TimerB timeout flag 
	}
}
void delay_ms(uint32_t n)
{
	SYSCTL->RCGCTIMER |= 0x08; 	// enable clock to Timer Block 3
	TIMER3->CTL = 0; 			
	TIMER3->CFG = 0x04; 		
	TIMER3->TBMR = 0x02; 		
	TIMER3->TBILR = 16000 - 1; 
	TIMER3->ICR = 0x100; 		
	TIMER3->CTL |= 0x0100;		
	
	for(uint32_t i = 0; i < n; i++)
	{
		while ((TIMER3->RIS & 0x100) == 0) ; // wait for TimerB timeout flag 1ms 
		TIMER3->ICR = 0x100; 				 // clear the TimerB timeout flag
	}
}

void delay_lcd(uint32_t d) {
	uint32_t i;
	for(i = 0; i < d; i++) {
		__asm__("nop");
	}
}
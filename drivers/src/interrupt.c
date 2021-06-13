#include "interrupt.h"

int8_t ready_to_walk = 0;

void portf_pb_interrupt_init(push_button_t button)
{
	__disable_irq();
	push_button_init(button); // init the button

    GPIOF->IS &= ~button;  // PF4 is edge-sensitive
    GPIOF->IBE &= ~button; // PF4 is not both edges
    GPIOF->IEV &= ~button; // PF4 falling edge event
    GPIOF->ICR |= button;  // clear flag
    GPIOF->IM |= button;   // arm interrupt

    NVIC->IP[30] = 0xA0; // priority 5
    NVIC_EnableIRQ(30);  // enable interrupt 30 in NVIC
	
	__enable_irq(); 	 // global enable IRQs 
	__enable_fault_irq();
}

void GPIOF_Handler()
{
    if (!push_button_status(SW1))
    {
        GPIOF->ICR = SW1; // acknowledge
        ready_to_walk = 1;
    }
    else
    {
        // Interrput from SW2
        GPIOF->ICR = SW2; // acknowledge
        // may be added in the future
    }
	GPIOF->ICR |= 0x11; 	// clear the interrupt flag before return //
}
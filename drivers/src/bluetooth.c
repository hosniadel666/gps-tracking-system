#include "bluetooth.h"

void bluetooth_init(void)
{
	double I_FBRD;
	SYSCTL->RCGCUART |= 0x04; // provide clock to UART2
	SYSCTL->RCGCGPIO |= 0x08; // Enable clock to PORTD
	
	GPIOD->LOCK = 0x4C4F434B; // Unlock commit register   
	GPIOD->CR = 0x80;  		  // Unlock pin D7
	
	UART2->CTL = 0; 		  // Disable UART2

	I_FBRD = (16000000 / 16.0) / BR;
	UART2->IBRD = (uint32_t)I_FBRD; 
	I_FBRD = (I_FBRD - (uint32_t)I_FBRD) * 1000;
	
	UART2->FBRD = (uint32_t)I_FBRD; 
	UART2->CC = 0; 			   // Use system clock 
	UART2->LCRH = 0x60; 	   // 8-bit, no parity, 1-stop bit, no FIFO 
	UART2->CTL = 0x301;        // Enable UART2, TXE, RXE 

	GPIOD->DEN |= 0XC0;  	   // Make PD6 and PD7 as digital 
	GPIOD->AFSEL |= 0XC0; 	   // Use PD6,PD7 alternate function 
	GPIOD->PCTL |= 0X11000000; // configure PD6 and PD7 for UART 
	
	delay_ms(1); 		   // wait for output line to stabilize 
}

void bluetooth_send_byte(uint8_t c) 
{
  while ((UART2->FR & 0x20) != 0)
    ;            // wait until Tx buffer not full
  UART2->DR = c; // before giving it another byte
}

uint8_t bluetooth_read_byte(void) 
{
  uint8_t c;

  while ((UART2->FR & 0x10) != 0)
    ;            // wait until the buffer is not empty
  c = UART2->DR; // read the received data

  return c;
}
void bluetooth_send_msg(char *msg)
{
	bluetooth_init();
	for(uint32_t i = 0; i < strlen(msg); i++)
	{
		bluetooth_send_byte(msg[i]);
	}
}


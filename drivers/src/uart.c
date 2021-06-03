#include "uart.h"

void uart0_init(void)
{
	double I_FBRD;
	SYSCTL->RCGCUART |= 1; // provide clock to UART0 
	SYSCTL->RCGCGPIO |= 1; // enable clock to PORTA 
	
	// UART0 initialization 
	UART0->CTL = 0; 	   // disable UART0 
	
	I_FBRD = (16000000 / 16.0) / BR;
	UART0->IBRD = (uint32_t) I_FBRD;	// Integer baudrate devisor
	
	I_FBRD = (I_FBRD - (uint32_t)I_FBRD) * 1000;
	UART0->FBRD = (uint32_t) I_FBRD;    // float baudrate devisor
	
	UART0->CC = 0; 		   // use system clock 
	
	UART0->LCRH = 0x60;    // 8-bit, no parity, 1-stop bit, no FIFO 
	UART0->CTL = 0x301;    // enable UART0, TXE, RXE 

	
	// UART0 TX0 and RX0 use PA0 and PA1. Set them up. 
	GPIOA->DEN = 0x03; 	   // Make PA0 and PA1 as digital 
	GPIOA->AFSEL = 0x03;   // Use PA0,PA1 alternate function 
	GPIOA->PCTL = 0x11;    // configure PA0 and PA1 for UART 
	
	delay_ms(1); 		   // wait for output line to stabilize 
}

void uart0_tx(uint8_t c)
{
	while((UART0->FR & 0x20) != 0); // wait until Tx buffer not full 
	UART0->DR = c; 					// before giving it another byte 
}

uint8_t uart0_rx(void)
{
	uint8_t c;
	
	while((UART0->FR & 0x10) != 0); // wait until the buffer is not empty 
	c = UART0->DR; 					// read the received data 
	
	return c; 
}

void uart1_init(void)
{
	double I_FBRD;
	SYSCTL->RCGCUART |= 2; // provide clock to UART1 
	SYSCTL->RCGCGPIO |= 2; // enable clock to PORTB 
	
	// UART1 initialization 
	UART1->CTL = 0; 	   // disable UART1
	
	I_FBRD = (16000000 / 16.0) / BR;
	UART1->IBRD = (uint32_t) I_FBRD;	// Integer baudrate devisor
	
	I_FBRD = (I_FBRD - (uint32_t)I_FBRD) * 1000;
	UART1->FBRD = (uint32_t) I_FBRD;    // float baudrate devisor
	
	UART1->CC = 0; 		   // use system clock 
	
	UART1->LCRH = 0x60;    // 8-bit, no parity, 1-stop bit, no FIFO 
	UART1->CTL = 0x301;    // enable UART1, TXE, RXE 

	// UART1 TX1 and RX1 use PB0 and PB1. Set them up. 
	GPIOB->DEN = 0x03; 	   // Make PB0 and PB1 as digital 
	GPIOB->AFSEL = 0x03;   // Use PB0,PB1 alternate function 
	GPIOB->PCTL = 0x11;    // configure PB0 and PB1 for UART 
	
	delay_ms(1); 		   // wait for output line to stabilize 
}


void uart1_tx(uint8_t c)
{
	while((UART1->FR & 0x20) != 0); // wait until Tx buffer not full 
	UART1->DR = c; 					// before giving it another byte 
}


uint8_t uart1_rx(void)
{
	uint8_t c;
	
	while((UART1->FR & 0x10) != 0); // wait until the buffer is not empty 
	c = UART1->DR; 					// read the received data 
	
	return c; 
}

void print_msg(char *msg)
{
	uart0_init();
	for(uint32_t i = 0; i < strlen(msg); i++)
	{
		uart0_tx(msg[i]);
	}
}
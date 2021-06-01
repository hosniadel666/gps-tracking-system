#include "TM4C123.h"
#include "i2c.h"

void I2C1_init(void)
{
	SYSCTL->RCGCI2C |= 0x02; 	// Enable clock to I2C1 
	SYSCTL->RCGCGPIO |= 0x01; 	// Enable clock to GPIOA 
	
	GPIOA->AFSEL |= 0xC0; 		// PA7, PA6 for I2C1
	GPIOA->PCTL &= ~0xFF000000; 
	GPIOA->PCTL |= 0x33000000;
	GPIOA->DEN |= 0xC0; 
	GPIOA->ODR |= 0x80; 
	I2C1->MCR = 0x10; 			// Master mode */
	I2C1->MTPR = 7;				// I2C bus clock 100 kHz @ 16 MHz 
}

int I2C_wait_till_done(void)
{
	while(I2C1->MCS & 1);		 // Wait until I2C master is not busy 
	return I2C1->MCS & 0xE; 	 // Return I2C error code 
}

char I2C1_byteWrite(int slave_addr, char mem_addr, char data)
{
	char error;

	I2C1->MSA = slave_addr << 1;
	I2C1->MDR = mem_addr;
	I2C1->MCS = 3;
	error = I2C_wait_till_done(); 
	if (error) return error;

	I2C1->MDR = data; 				
	I2C1->MCS = 5; 
	error = I2C_wait_till_done(); 
	
	while(I2C1->MCS & 0x40);
	error = I2C1->MCS & 0xE;
	if (error) return error;
	return 0; 
}
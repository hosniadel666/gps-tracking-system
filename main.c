#include <stdint.h>
#include <stdio.h>
#include "TM4C123.h"
#include "push_button.h"
#include "delay_timer.h"
#include "led.h"
#include "display.h"
#include "gps.h"
#include "uart.h"

int main(void)
{	
	uart1_init();
	uart0_init();
	
	geographic_point_t point;
	char data, buffer[100], RMC_code[3];
	unsigned char is_RMC_string = 0;
	unsigned char RMC_index = 0;
	unsigned char is_RMC_received_completely = 0;

	while(1)
	{
		data = uart1_rx();			
	
		if(data == '$')
		{
			is_RMC_string = 0;
			RMC_index = 0;
		}
		else if(is_RMC_string ==1)
		{
			buffer[RMC_index++] = data;
			//uart0_tx(dat);
			if(data == '\n')
				is_RMC_received_completely = 1;
		}
		else if(RMC_code[0] == 'R' && RMC_code[1] == 'M' && RMC_code[2] == 'C')
		{
			is_RMC_string = 1;
			RMC_code[0] = 0; 
			RMC_code[0] = 0;
			RMC_code[0] = 0;		
		}
		else
		{
			RMC_code[0] = RMC_code[1];
			RMC_code[1] = RMC_code[2];
			RMC_code[2] = data;
		}
		if(is_RMC_received_completely == 1){

			print_msg(buffer);
			print_msg("\n\r");
			point = parser(buffer);
			is_RMC_received_completely = 0;
		}
	}

}


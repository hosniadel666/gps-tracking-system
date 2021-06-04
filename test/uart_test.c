#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "UART.h"
#include <delay_timer.h>
#include <TM4C123.h>


int main_uart(void){
	
		char *name = "hussien";
	
		while(1){
			
			print_msg(name);
			print_msg("\n\r");
			delay_ms(500);
			
		}
	
}

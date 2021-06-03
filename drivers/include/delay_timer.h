#ifndef __DELAYTIMER_H__
#define __DELAYTIMER_H__

#include "TM4C123.h"
#include <stdint.h>

/**
* @brief Configure the Timer3 to achieve delay in micros 
*
* @param n the time in microsec needed for delay
* 
* @return void
*/
void delay_micros(uint32_t n);

/**
* @brief Configure the Timer2 to achieve delay in millis 
*
* @param n the time in millisec needed for delay
* 
* @return void
*/
void delay_ms(uint32_t n);


void delay_lcd(uint32_t d);
#endif
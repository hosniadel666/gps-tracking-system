#ifndef __UART_H__
#define __UART_H__

#include <string.h>
#include <stdint.h>

#include "TM4C123.h"
#include "delay_timer.h"

#define BR 9600.0   // Baud Rate

/**
* @brief Configure the uart pins to use them
* 
* Here we will configure the UART0 which connected
* to the ICDL (UART0RX --> PA0, UART0TX --> PA1)
*
* @param void
* 
* @return void
*/
void uart0_init(void);

/**
* @brief Transfer the data using TX pin of UART0
* 
* @param c 8-bits variable which hold data need to send
* 
* @return void
*/
void uart0_tx(uint8_t c);

/**
* @brief Recieve the data using RX pin of UART0
* 
* @param void
* 
* @return 8-bits variable which hold data need to recieve
*/
uint8_t uart0_rx(void);

/**
* @brief Configure the uart pins to use them
* 
* Here we will configure the UART0 which connected
* to the ICDL (UART0RX --> PA0, UART0TX --> PA1)
*
* @param void
* 
* @return void
*/
void uart0_init(void);

/**
* @brief Transfer the data using TX pin of UART0
* 
* @param c 8-bits variable which hold data need to send
* 
* @return void
*/
void uart0_tx(uint8_t c);

/**
* @brief Recieve the data using RX pin of UART0
* 
* @param void
* 
* @return 8-bits variable which hold data need to recieve
*/
uint8_t uart0_rx(void);
/**
* @brief Configure the uart pins to use them
* 
* Here we will configure the UART0 which connected
* to the ICDL (UART0RX --> PA0, UART0TX --> PA1)
*
* @param void
* 
* @return void
*/
void uart1_init(void);

/**
* @brief Transfer the data using TX pin of UART0
* 
* @param c 8-bits variable which hold data need to send
* 
* @return void
*/
void uart1_tx(uint8_t c);

/**
* @brief Recieve the data using RX pin of UART0
* 
* @param void
* 
* @return 8-bits variable which hold data need to recieve
*/
uint8_t uart1_rx(void);


/**
* @brief Print msg using uart
* 
* @param pointer to char
* 
* @return void
*/
void print_msg(char *msg);

#endif
#ifndef __BLUETOOTH__
#define __BLUETOOTH__

#include <stdint.h>
#include <string.h>

#include "TM4C123.h"
#include "delay_timer.h"

#define BR 9600.0 // Baud Rate

/**
 * @brief Configure the UART pins
 *
 * Here we will configure UART5 which connected
 * to the ICDL (UART5RX --> PE4, UART5TX --> PE5)
 *
 * @param void
 *
 * @return void
 */
void bluetooth_init(void);

/**
 * @brief Transfer the data using TX pin of UART5
 *
 * @param c 8-bits variable which hold data need to send
 *
 * @return void
 */
void bluetooth_send_byte(uint8_t c);

/**
 * @brief Recieve the data using RX pin of UART5
 *
 * @param void
 *
 * @return 8-bits variable which hold data need to recieve
 */
uint8_t bluetooth_read_byte(void);

#endif // __BLUETOOTH__

#include "bluetooth.h"

void bluetooth_init(void) {
  double I_FBRD;
  SYSCTL->RCGCUART |= (1 << 5); // provide clock to UART5
  SYSCTL->RCGCGPIO |= (1 << 5); // enable clock to PORTE

  // UART5 initialization
  UART5->CTL = 0; // disable UART5

  I_FBRD = (16000000 / 16.0) / BR;
  UART5->IBRD = (uint32_t)I_FBRD; // Integer baudrate devisor

  I_FBRD = (I_FBRD - (uint32_t)I_FBRD) * 1000;
  UART5->FBRD = (uint32_t)I_FBRD; // float baudrate devisor

  UART5->CC = 0; // use system clock

  UART5->LCRH = 0x60; // 8-bit, no parity, 1-stop bit, no FIFO
  UART5->CTL = 0x301; // enable UART5, TXE, RXE

  // UART5 TX5 and RX5 use PE4 and PE5. Set them up.
  GPIOE->DEN = (1 << 4 | 1 << 5);   // Make PE4 and PE5 as digital
  GPIOE->AFSEL = (1 << 4 | 1 << 5); // Use PE4,PE5 alternate function
  GPIOE->PCTL |= (1 << 4 | 1 << 5); // configure PE4 and PE5 for UART

  delay_ms(1); // wait for output line to stabilize
}

void bluetooth_send_byte(uint8_t c) {
  while ((UART5->FR & 0x20) != 0)
    ;            // wait until Tx buffer not full
  UART5->DR = c; // before giving it another byte
}

uint8_t bluetooth_read_byte(void) {
  uint8_t c;

  while ((UART5->FR & 0x10) != 0)
    ;            // wait until the buffer is not empty
  c = UART5->DR; // read the received data

  return c;
}

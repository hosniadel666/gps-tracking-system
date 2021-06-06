#include <stdint.h>

#include "TM4C123.h"
#include "delayTimer.h"
#include "ultrasonic.h"


/**
 * @file ultrasonic.c
 * @brief
 *
 * @author Hosni Adel
 * @date Mar  2021
 *
 */

/***********************************************************
                                Function Definitions
***********************************************************/

void ultrasonic_init(ultrasonicPFConfig pins) {
  // Enable clock to required ports
  SYSCTL->RCGCGPIO |= 0x02;
  SYSCTL->RCGCTIMER |= 0x01;
  SYSCTL->RCGCGPIO |= 2;

  GPIOB->DIR |= pins.TRIG;
  GPIOB->DEN |= pins.TRIG;

  GPIOB->DIR &= ~pins.ECHO;
  GPIOB->DEN |= pins.ECHO;
  GPIOB->AFSEL |= pins.ECHO;  // use PB6 alternate function
  GPIOB->PCTL &= ~0x0F000000; // configure PB6 for T0CCP0 PMC6 = 0x7
  GPIOB->PCTL |= 0x07000000;

  TIMER0->CTL &= ~1;   // disable timer0A during setup
  TIMER0->CFG = 4;     // 16-bit timer mode
  TIMER0->TAMR = 0x17; // up-count | edge-time | capture mode
  TIMER0->CTL |= 0x0c; // capture the rising edge
  TIMER0->CTL |= 1;    // enable timer0A
}

float ultrasonic_measure(void) {

  uint32_t fallingEdge, raisingEdge, pulseWidth;
  float distance;

  // trigger the trigger pin
  GPIOB->DATA &= ~0x80;
  delayMs(30);
  GPIOB->DATA |= 0x80;
  delayMicros(10);
  GPIOB->DATA &= ~0x80;

  // capture the first rising edge
  TIMER0->ICR = 4; // clear timer0A capture flag
  while ((TIMER0->RIS & 0x04) == 0 && (TIMER0->RIS & 0x1) == 0)
    ; // wait till captured
  fallingEdge = TIMER0->TAR;

  // capture the first faling edge
  TIMER0->ICR = 4;
  while ((TIMER0->RIS & 0x04) == 0 && (TIMER0->RIS & 0x1) == 0)
    ;
  raisingEdge = TIMER0->TAR;

  pulseWidth = (raisingEdge - fallingEdge) & 0x00FFFFFF;

  distance = pulseWidth * (1.0 / (SystemCoreClock)) *
             34000; // soundspeed = distance / time , time = timer clock cycles
                    // * system clock period

  return distance;
}

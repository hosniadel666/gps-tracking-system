#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include "TM4C123.h"
#include "push_button.h"

void portf_pb_interrupt_init(push_button_t);

void GPIOF_Handler();

#endif
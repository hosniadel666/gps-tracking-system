#ifndef __PUSHBUTTON_H__
#define __PUSHBUTTON_H__

#include "TM4C123.h"
#include <stdint.h>

typedef enum
{
	SW1 = 0x10,
	SW2 = 0x01
} push_button_t;

void push_button_init(push_button_t );

int push_button_status(push_button_t );

#endif

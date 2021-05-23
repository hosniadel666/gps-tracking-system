#ifndef __PUSHBUTTON_H__
#define __PUSHBUTTON_H__

#include "TM4C123.h"
#include <stdint.h>

typedef enum{
	SW1 = 0x10,
	SW2 = 0x01
} pushButton;

void pushButton_init(pushButton button);

int pushButton_status(pushButton button);

#endif


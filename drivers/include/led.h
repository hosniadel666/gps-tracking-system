#ifndef __LED_H__
#define __LED_H__

#include "TM4C123.h"
#include <stdint.h>

typedef enum
{
	RED = 0x02,
	BLUE = 0x04,
	GREEN = 0x08
} led_t;

void led_init(led_t );

void led_on(led_t );

void led_off(led_t );

void all_on();

void all_off();

#endif
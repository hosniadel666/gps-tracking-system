#ifndef __LEDS_H__
#define __LEDS_H__

typedef enum{
	RED = 0x02,
	BLUE = 0x04,
	GREEN = 0x08
}LED;

void led_init(LED led);

void led_on(LED led);
	
void led_off(LED led);

void all_on();

void all_off();

#endif
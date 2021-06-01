#include <stdint.h>
#include <stdio.h>

#include "TM4C123.h"
#include "push_button.h"
#include "delay_timer.h"
#include "led.h"
#include "display.h"
#include "gps.h"
#include "uart.h"

#define MAX_INVAILD 10
#define MAX_DISTANCE 100.0

int main(void)
{
    uart1_init();
    uart0_init();

    geographic_point_t curr_p;
    geographic_point_t last_p;

    float distance = 0.0;
    int num_invaild = 0;

    // wait untail gps is active
    last_p = get_geographic_point();
    while (!last_p.is_vaild)
        last_p = get_geographic_point();

    while (1)
    {
        curr_p = get_geographic_point();
        if (curr_p.is_vaild)
        {
            num_invaild--;
            distance += distance_sphere(&last_p, &curr_p);
            last_p = curr_p;
        }
        else
        {
            num_invaild++;
        }

        // reached the requird destination
        if (distance >= MAX_DISTANCE)
        {
            led_on(RED);
        }

        // chech for GPS faults limits
        if (num_invaild < 0)
        {
            num_invaild = 0;
        }
        else if (num_invaild >= MAX_INVAILD)
        {
            num_invaild = MAX_INVAILD;
            // display error "gps has stoped"
        }
    }
}

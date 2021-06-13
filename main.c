#include <stdint.h>
#include <stdio.h>

#include "TM4C123.h"
#include "push_button.h"
#include "delay_timer.h"
#include "led.h"
#include "display.h"
#include "gps.h"
#include "uart.h"
#include "interrupt.h"
#include "bluetooth.h"

#define MAX_INVAILD 10
#define MAX_DISTANCE 100.0
#define USING_WAITING_MODE

extern int8_t ready_to_walk;

void system_gpio_init();
void waiting_mode();
void searching_mode();
void reading_mode();
void reaching_mode();
void error_mode();
void display_distance(double);

int main(void)
{
    system_gpio_init();

    geographic_point_t curr_p;
    geographic_point_t last_p;
    double distance = 0.0;
    int32_t num_invaild = 0;
    uint32_t reach_target = 0;

    // wait untail gps is active
    last_p = get_geographic_point();
	
    while (!last_p.is_vaild)
    {
        last_p = get_geographic_point();
        searching_mode();
    }

    while (1)
    {
#ifdef USING_WAITING_MODE
        // wait until push button SW1 pressed
        if (!ready_to_walk)
        {
            waiting_mode();
            continue;
        }
#endif
		
        curr_p = get_geographic_point();
        if (curr_p.is_vaild)
        {
            distance += distance_sphere(&last_p, &curr_p);
			
            display_distance(distance);
			char bluetooth_msg[30];
		
			sprintf(bluetooth_msg, "%f,%f\n", curr_p.lat, curr_p.lon);
			bluetooth_send_msg(bluetooth_msg);	// Send coordinates to bluetooth module
			print_msg(bluetooth_msg); 			// Send coordinates to PC at real time

            last_p = curr_p;
            num_invaild--;
        }
        else
        {
            num_invaild++;
            if (!reach_target)
                searching_mode();
        }

        // reached the requird destination
        if (distance >= MAX_DISTANCE)
        {
            reaching_mode();
            reach_target = 1;
			print_msg("end\n"); // send end to pc to draw the stored coordinates
        }

        // check for GPS faults limits
        if (num_invaild < 0)
        {
            num_invaild = 0;
        }
        else if (num_invaild >= MAX_INVAILD)
        {
            // GPS has reached the max invaild points 
            num_invaild = MAX_INVAILD;
			error_mode();

        }
    }
}

void system_gpio_init()
{
    terminal_init(); // For communication btw PC and tiva
    gps_init();		 // For communication btw tiva and gps module
    lcd_init();  	 // For displaying measured data
	bluetooth_init();// For communication btw bluetooth module and mobile 
    portf_pb_interrupt_init(SW1); // Enable Intruupt on pushbutton 1
    led_init(RED | GREEN | BLUE); // Initialize all leds
}
 
void display_distance(double distance)
{
    char lcd_distance[20];

    lcd_clean();
	
    sprintf(lcd_distance, "Distance:");
    lcd_set_cursor(0, 0);
    lcd_print(lcd_distance, strlen(lcd_distance));

    sprintf(lcd_distance, "<G16>");
    lcd_set_cursor(0, 11);
    lcd_print(lcd_distance, strlen(lcd_distance));

    sprintf(lcd_distance, "%0.4f m", distance);
    lcd_set_cursor(1, 3);
    lcd_print(lcd_distance, strlen(lcd_distance));
}

void waiting_mode()
{
    lcd_clean();
    char message[20];

    lcd_set_cursor(0, 5);
    sprintf(message, "GPS Ready");
    lcd_print(message, strlen(message));

    lcd_set_cursor(1, 2);
    sprintf(message, "Push SW1 to start");
    lcd_print(message, strlen(message));
	delay_ms(800);
}

void searching_mode()
{
    static int counter = 0;
    char ptr[20];

    all_off();
    led_on(GREEN);

    // I also don't like this delay # hassan
     delay_ms(400);
     led_off(GREEN);
     delay_ms(400);

    if (counter == 1)
    {
        sprintf(ptr, "Searching..");
    }
    else if (counter == 2)
    {
        sprintf(ptr, "Searching...");
    }
    else if (counter == 3)
    {
        sprintf(ptr, "Searching....");
    }
    else
    {
        lcd_clean();
        sprintf(ptr, "Searching.");
        counter = 0;
    }

    lcd_set_cursor(0, 0);
    lcd_print(ptr, strlen(ptr));
    counter++;
}


void reading_mode()
{
    all_off();
    led_on(BLUE);
    // I don't like this delay # hassan
    delay_ms(400);
    led_off(BLUE);
    delay_ms(400);
}

void reaching_mode()
{
    all_off();
    led_on(RED);
}

void error_mode()
{     
	lcd_clean();
	lcd_set_cursor(0, 0);
	char stop_message[20];
	sprintf(stop_message, "Error: GPS Stoped");
	lcd_print(stop_message, strlen(stop_message));
	delay_ms(800);
}
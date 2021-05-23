#include <stdint.h>
#include "TM4C123.h"
#include "gps.h"


/***********************************************************
				Function Definitions
***********************************************************/
void gps_init()
{
	uart0_init();
}

double distance_sphere(double lat1, double lon1,
                       double lat2, double lon2)
{

}

double distance_spheroid(double lat1, double lon1,
                         double lat2, double lon2)
{

}

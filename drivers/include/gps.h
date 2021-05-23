#ifndef __GPS_H__
#define __GPS_H__

#include <stdint.h>
#include "uart.h"

#define PI 3.14159265358979323846

void gps_init();

double distance_sphere(double lat1, double lon1,
                       double lat2, double lon2);

double distance_spheroid(double lat1, double lon1,
                         double lat2, double lon2);

#endif

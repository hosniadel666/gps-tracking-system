#ifndef __GPS_H__
#define __GPS_H__

#include <stdint.h>
#include "uart.h"

#define PI 3.14159265358979323846

typedef struct
{
    double lat;
    double lon;

} geographic_point;

void gps_init();

double distance_sphere(geographic_point *p1, geographic_point *p2);

double distance_spheroid(geographic_point *p1, geographic_point *p2);

#endif

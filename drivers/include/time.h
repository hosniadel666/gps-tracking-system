#ifndef __TIME_H__
#define __TIME_H__

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

typedef struct
{
    uint8_t csec;
    uint8_t sec;
    uint8_t min;
    uint8_t hr;
} time_pt;

time_pt get_time(char *);

float time_difference(time_pt *, time_pt *);

#endif
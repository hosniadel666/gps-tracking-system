#include "time.h"

// parse time from format hhmmss.csec
time_pt get_time(char *time_str)
{
    time_pt time_point;

    if (*time_str == '-')
        time_str++;

    uint32_t time = atoi(time_str);

    while (isdigit(*time_str++))
        ;

    if (time_str[-1] == '.' && isdigit(time_str[0]))
    {
        time_point.csec = 10 * (time_str[0] - '0');
        if (isdigit(time_str[1]))
            time_point.csec += time_str[1] - '0';
    }
    else
    {
        time_point.csec = 0;
    }

    time_point.sec = time % 100;
    time_point.min = (time / 100) % 100;
    time_point.hr = (time / 10000) % 100;

    return time_point;
}

// return time difference (t1 - t2) in secs
float time_difference(time_pt *t1, time_pt *t2)
{
    float csec = t1->csec - t2->csec;
    int sec = t1->sec - t2->sec;
    int min = t1->min - t2->min;
    int hr = t1->hr - t2->hr;

    float diff = sec + min * 60 + hr * 60 * 60;
    diff += csec / 100.0;

    return diff;
}
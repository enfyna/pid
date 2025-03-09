
#include "car.h"
#include <string.h>

static void _get_power_curve(car* c);

car* get_rc_car(){
    const int GEAR_COUNT = 1;
    const int MAX_RPM = 8000;

    car* rc_car = _init_car(MAX_RPM, GEAR_COUNT);
    memcpy(rc_car->name, "RC Car", 7);

    rc_car->mass = 100;
    rc_car->wheel_radius = 1.0;
    rc_car->gear_shift_time = 3;
    rc_car->wheel_radius = 0.289052;

    rc_car->input.gear = 2;
    rc_car->input.throttle = 1;

    rc_car->current.shift_time_left = 0;

    rc_car->gear_ratios[0] = -1.000;
    rc_car->gear_ratios[1] =  0.000;
    rc_car->gear_ratios[2] =  3.000;

    rc_car->final_drive_ratio = 4.300;

    _get_power_curve(rc_car);

    return rc_car;
}

static void _get_power_curve(car* c) {
    for (int i = 1000; i < c->max_rpm; i+=100) {
        for (int j = 0; j < 100; j++) {
            c->power_curve[i + j] = 100.0;
        }
    }
}

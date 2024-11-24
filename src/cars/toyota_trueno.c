
#include "car.h"

static void _get_toyota_trueno_power_curve(car* c);

car* get_toyota_trueno(){
    const int GEAR_COUNT = 5;
    const int MAX_RPM = 8000;

    car* toyota = _init_car(MAX_RPM, GEAR_COUNT);

    toyota->mass = 1089;
    toyota->wheel_radius = 1.0;
    toyota->gear_shift_time = 3;
    toyota->wheel_radius = 0.289052;

    toyota->input.gear = 2;
    toyota->input.throttle = 1;

    toyota->current.shift_time_left = 0;

    toyota->gear_ratios[0] = -1.000;
    toyota->gear_ratios[1] =  0.000;
    toyota->gear_ratios[2] =  3.587;
    toyota->gear_ratios[3] =  2.022;
    toyota->gear_ratios[4] =  1.384;
    toyota->gear_ratios[5] =  1.000;
    toyota->gear_ratios[6] =  0.861;

    toyota->final_drive_ratio = 4.300;

    _get_toyota_trueno_power_curve(toyota);

    return toyota;
}

static void _get_toyota_trueno_power_curve(car* c) {
    c->power_curve[1000] = 8.9;
    c->power_curve[1100] = 11.2;
    c->power_curve[1200] = 13.4;
    c->power_curve[1300] = 15.6;
    c->power_curve[1400] = 17.9;
    c->power_curve[1500] = 20.1;
    c->power_curve[1600] = 22.3;
    c->power_curve[1700] = 24.5;
    c->power_curve[1800] = 26.8;
    c->power_curve[1900] = 29.0;
    c->power_curve[2000] = 31.3;
    c->power_curve[2100] = 33.5;
    c->power_curve[2200] = 35.7;
    c->power_curve[2300] = 38.0;
    c->power_curve[2400] = 40.2;
    c->power_curve[2500] = 42.4;
    c->power_curve[2600] = 44.7;
    c->power_curve[2700] = 46.9;
    c->power_curve[2800] = 49.1;
    c->power_curve[2900] = 51.4;
    c->power_curve[3000] = 53.6;
    c->power_curve[3100] = 55.8;
    c->power_curve[3200] = 58.1;
    c->power_curve[3300] = 60.3;
    c->power_curve[3400] = 62.5;
    c->power_curve[3500] = 64.7;
    c->power_curve[3600] = 67.0;
    c->power_curve[3700] = 69.2;
    c->power_curve[3800] = 71.5;
    c->power_curve[3900] = 73.7;
    c->power_curve[4000] = 75.9;
    c->power_curve[4100] = 78.1;
    c->power_curve[4200] = 80.4;
    c->power_curve[4300] = 82.6;
    c->power_curve[4400] = 84.8;
    c->power_curve[4500] = 87.1;
    c->power_curve[4600] = 89.3;
    c->power_curve[4700] = 91.5;
    c->power_curve[4800] = 93.8;
    c->power_curve[4900] = 96.0;
    c->power_curve[5000] = 98.2;
    c->power_curve[5100] = 100.2;
    c->power_curve[5200] = 102.0;
    c->power_curve[5300] = 103.8;
    c->power_curve[5400] = 105.6;
    c->power_curve[5500] = 107.2;
    c->power_curve[5600] = 108.8;
    c->power_curve[5700] = 110.2;
    c->power_curve[5800] = 111.7;
    c->power_curve[5900] = 112.9;
    c->power_curve[6000] = 114.2;
    c->power_curve[6100] = 115.3;
    c->power_curve[6200] = 116.3;
    c->power_curve[6300] = 117.2;
    c->power_curve[6400] = 118.0;
    c->power_curve[6500] = 118.7;
    c->power_curve[6600] = 119.3;
    c->power_curve[6700] = 118.7;
    c->power_curve[6800] = 117.4;
    c->power_curve[6900] = 114.9;
    c->power_curve[7000] = 111.6;
    c->power_curve[7100] = 107.3;

    for (int i = 1000; i < c->max_rpm; i+=100) {
        for (int j = 0; j < 100; j++) {
            c->power_curve[i + j] = c->power_curve[i]
                + (c->power_curve[i + 100] - c->power_curve[i]) * j / 100.0;
        }
    }
}

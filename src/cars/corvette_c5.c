
#include "car.h"

static void _get_corvette_c5_power_curve(car* c);

car* get_corvette_c5() {
    const int GEAR_COUNT = 6;
    const int MAX_RPM = 8000;

    car *corvette = _init_car(MAX_RPM, GEAR_COUNT);

    corvette->mass = 1500;
    corvette->gear_shift_time = 2;
    corvette->wheel_radius = 0.33;

    corvette->input.gear = 2;
    corvette->input.throttle = 1;

    corvette->current.shift_time_left = 0;

    corvette->gear_ratios[0] = -2.90;
    corvette->gear_ratios[1] =  0.00;
    corvette->gear_ratios[2] =  3.06;
    corvette->gear_ratios[3] =  1.78;
    corvette->gear_ratios[4] =  1.30;
    corvette->gear_ratios[5] =  1.00;
    corvette->gear_ratios[6] =  0.74;
    corvette->gear_ratios[7] =  0.50;

    corvette->final_drive_ratio = 3.42;

    _get_corvette_c5_power_curve(corvette);

    return corvette;
}

void _get_corvette_c5_power_curve(car* c) {
    c->power_curve[1000] = 220;
    c->power_curve[1100] = 250;
    c->power_curve[1200] = 275;
    c->power_curve[1300] = 296.2;
    c->power_curve[1400] = 314.3;
    c->power_curve[1500] = 330;
    c->power_curve[1600] = 343.8;
    c->power_curve[1700] = 355.9;
    c->power_curve[1800] = 366.7;
    c->power_curve[1900] = 376.3;
    c->power_curve[2000] = 385;
    c->power_curve[2100] = 392.9;
    c->power_curve[2200] = 400;
    c->power_curve[2300] = 406.5;
    c->power_curve[2400] = 412.5;
    c->power_curve[2500] = 418;
    c->power_curve[2600] = 423.1;
    c->power_curve[2700] = 427.8;
    c->power_curve[2800] = 432.1;
    c->power_curve[2900] = 436.2;
    c->power_curve[3000] = 440;
    c->power_curve[3100] = 443.5;
    c->power_curve[3200] = 446.9;
    c->power_curve[3300] = 450;
    c->power_curve[3400] = 452.9;
    c->power_curve[3500] = 455.7;
    c->power_curve[3600] = 458.3;
    c->power_curve[3700] = 460.8;
    c->power_curve[3800] = 463.2;
    c->power_curve[3900] = 465.4;
    c->power_curve[4000] = 467.5;
    c->power_curve[4100] = 469.5;
    c->power_curve[4200] = 471.4;
    c->power_curve[4300] = 473.3;
    c->power_curve[4400] = 475;
    c->power_curve[4500] = 474.7;
    c->power_curve[4600] = 474;
    c->power_curve[4700] = 472.7;
    c->power_curve[4800] = 470.9;
    c->power_curve[4900] = 468.6;
    c->power_curve[5000] = 465.8;
    c->power_curve[5100] = 462.5;
    c->power_curve[5200] = 458.7;
    c->power_curve[5300] = 454.3;
    c->power_curve[5400] = 449.5;
    c->power_curve[5500] = 444.1;
    c->power_curve[5600] = 438.2;
    c->power_curve[5700] = 428.8;
    c->power_curve[5800] = 416.3;
    c->power_curve[5900] = 401;
    c->power_curve[6000] = 382.8;

    for (int i = 1000; i < c->max_rpm; i+=100) {
        for (int j = 0; j < 100; j++) {
            c->power_curve[i + j] = c->power_curve[i]
                + (c->power_curve[i + 100] - c->power_curve[i]) * j / 100.0;
        }
    }
}

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"
#include "car.h"

#define REVERSE 0
#define NEUTRAL 1

double max(double a, double b){
    return a > b ? a : b;
}

car* _init_car(int max_rpm, int gear_count){
    car *c = malloc(sizeof(car));
    assert(c != NULL);

    c->max_rpm = max_rpm;
    c->gear_count = gear_count;

    c->power_curve = calloc(c->max_rpm + 1, sizeof(float));
    c->gear_ratios = malloc(sizeof(double) * (c->gear_count + 2));
    assert(c->power_curve != NULL);
    assert(c->gear_ratios != NULL);

    c->current.position = 0;
    c->current.acceleration = 0;
    c->current.velocity = 0;

    return c;
}

car* get_car(const char* name){
    if (!strcmp(name, "toyota_trueno")) {
        return get_toyota_trueno();
    } else if(!strcmp(name, "corvette_c5")) {
        return get_corvette_c5();
    } else {
        return get_rc_car();
    }
}

void free_car(car* car){
    free(car->power_curve);
    free(car->gear_ratios);
    free(car);
}

double calculate_rpm(car* c){
    if (c->input.gear == NEUTRAL)
        return 1000.0;

    double wheel_rotation_rate = c->current.velocity / c->wheel_radius; // rad/s
    double rpm = wheel_rotation_rate
        * c->gear_ratios[c->input.gear]
        * c->final_drive_ratio
        * 60.0 / (2 * PI)
    ;

    return max(1000.0, rpm);
}

double _calculate_engine_force(car* c){
    c->current.rpm = calculate_rpm(c);
    if (c->current.rpm >= c->max_rpm) {
        c->current.rpm = c->max_rpm - 1;
    }
    const double power_factor = c->input.throttle
        * c->power_curve[(int)c->current.rpm]
    ;
    const double transmission_efficiency = 0.7;
    const double engine_force = power_factor
        * c->final_drive_ratio
        * c->gear_ratios[c->input.gear]
        * transmission_efficiency
        / c->wheel_radius
    ;
    return engine_force;
}

void simulate(car* c, double delta){
    double engine_force = _calculate_engine_force(c);
    double max_rear_traction = (c->mass / 2.0) * 9.8;
    if (engine_force > max_rear_traction) {
        engine_force = max_rear_traction;
    }

    const float drag_coefficient = 0.3f;
    const float air_density = 1.225f;
    const float cross_sectional_area = 2.2f;
    double drag = 0.5f
        * drag_coefficient
        * air_density
        * cross_sectional_area
    ;
    double rr = 30.0 * drag;

    drag *= pow(c->current.velocity, 2);
    rr *= c->current.velocity;

    double net_force = engine_force - drag - rr;

    c->current.acceleration = net_force / c->mass;
    c->current.velocity += delta * c->current.acceleration;
    c->current.position += delta * c->current.velocity;
} 

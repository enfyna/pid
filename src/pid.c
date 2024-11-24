
#include "pid.h"

int sign(double num){
    return num > 0 ? 1 : -1;
}

void run_pid(car* c, pid* p, double delta){
    double err = (p->target_speed / 3.6) - c->current.velocity;
    double p_k = p->P * err;

    double i_k = p->I * err * delta;
    p->it = p->it + i_k;

    double d_k = p->D * ( err - p->prev_err ) / delta;

    // amount of pressure on gas pedal
    double amount = p_k + p->it + d_k;
    if (amount > 1.0) {
        amount = 1.0;
    } else if (amount < -1.0) {
        amount = -1.0;
    }
    if (c->current.shift_time_left > 0) {
        c->current.shift_time_left -= delta;
    } else {
        if (amount > 0){
            if (c->input.gear < 3){
                c->input.gear += 1;
                c->current.shift_time_left = c->gear_shift_time;
            }
            else if ( c->current.acceleration <= 0 
                && c->current.rpm > c->max_rpm / 4 
                && c->input.gear < c->gear_count + 2){
                c->input.gear += 1;
                c->current.shift_time_left = c->gear_shift_time;
            }
        } 
        else if (amount < 0 
                && c->current.rpm < 3000 
                && c->input.gear > 3){
            c->input.gear -= 1;
            c->current.shift_time_left = c->gear_shift_time;
        }
    }

    c->input.throttle = amount;

    p->prev_err = err;
}


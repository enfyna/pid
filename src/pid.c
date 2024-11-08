
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
        if (c->current.acceleration < 0 && c->input.gear < c->gear_count + 2)
            c->input.gear += 1;
    } else if (amount < -1.0) {
        amount = -1.0;
        if (c->current.rpm < 2000 && c->input.gear > 2)
            c->input.gear -= 1;
    }
    c->input.throttle = amount;

    p->prev_err = err;
}


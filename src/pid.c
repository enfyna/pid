
#include "pid.h"

int sign(double num)
{
    return num > 0 ? 1 : -1;
}

void run_pid(car* c, pid* p, double delta)
{
    double err = (p->target_speed / 3.6) - c->current.velocity;
    double p_k = p->P * err;

    double i_k = p->I * err * delta;
    p->it = p->it + i_k;

    double d_k = p->D * (err - p->prev_err) / delta;

    double throttle = p_k + p->it + d_k;
    if (throttle > 1.0) {
        throttle = 1.0;
    } else if (throttle < -1.0) {
        throttle = -1.0;
    }
    if (c->current.shift_time_left > 0) {
        c->current.shift_time_left -= delta;
    } else {
        if (throttle > 0
            && c->current.acceleration <= 0
            // && c->current.rpm > c->max_rpm / 4
            && c->input.gear < c->gear_count + 2 - 1) {
            c->input.gear += 1;
            c->current.shift_time_left = c->gear_shift_time;

        } else if (throttle < 0
            && c->current.rpm < 3000
            && c->input.gear > 2) {
            c->input.gear -= 1;
            c->current.shift_time_left = c->gear_shift_time;
        }
    }

    c->input.throttle = throttle;

    p->prev_err = err;
}

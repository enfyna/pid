#ifndef PID_H
#define PID_H

#include "car.h"

typedef struct {
    double P;
    double I;
    double D;

    double target_speed;

    double prev_err;
    double it;
} pid;

void run_pid(car* c, pid* p, double delta);
#endif // PID_H

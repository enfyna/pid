#ifndef CAR_H
#define CAR_H

typedef struct {
    double rpm;
    int shift_time_left;

    double acceleration; // mps²
    double velocity;     // mps
    double position;     // m
} car_state;

typedef struct {
    int gear;
    float throttle;
} car_input;

typedef struct {
    double max_rpm;

    float *power_curve;
    double *gear_ratios;
    float final_drive_ratio;
    int gear_count;
    int gear_shift_time;

    float mass;         // kg
    float wheel_radius; // m

    car_state current;
    car_input input;
} car;

car* _get_car(int max_rpm, int gear_count);
void simulate(car* car, double delta);

car* get_toyota_trueno();
car* get_corvette_c5();
#endif // CAR_H

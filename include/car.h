#ifndef CAR_H
#define CAR_H

typedef struct {
    double rpm;
    float shift_time_left;

    double acceleration; // mps²
    double velocity;     // mps
    double position;     // m
} car_state;

typedef struct {
    int gear;
    float throttle;
} car_input;

typedef struct {
    char name[20];
    double max_rpm;

    float *power_curve;
    double *gear_ratios;
    float final_drive_ratio;
    int gear_count;
    float gear_shift_time;

    float mass;         // kg
    float wheel_radius; // m

    car_state current;
    car_input input;
} car;

car* get_car(const char* name);
car* _init_car(int max_rpm, int gear_count);
void simulate(car* car, double delta);
void free_car(car* car);

car* get_toyota_trueno();
car* get_corvette_c5();
car* get_rc_car();
#endif // CAR_H

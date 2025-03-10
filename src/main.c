#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#include "raylib.h"
#include "raymath.h"
#include "graph.h"
#include "pid.h"
#include "car.h"

#define HEIGHT 300 * 2
#define WIDTH  400 * 2

void run_sim(car* c, pid* p, int sim_time, line* speed_line, line* rpm_line, line* gear_line, line* throttle_line){
    c->input.gear = 1;
    c->input.throttle = 0.0;

    c->current.rpm = 0;
    c->current.velocity = 0;
    c->current.position = 0;
    c->current.acceleration = 0;
    c->current.shift_time_left = 0;

    p->it = 0;
    p->prev_err = 0;

    for (int i = 0; i < sim_time; i++) {
        simulate(c, 0.1);
        speed_line->points[i] = (Vector2){ i, c->current.velocity * 3.6 };
        gear_line->points[i] = (Vector2){ i, (c->input.gear) * 10 };
        rpm_line->points[i] = (Vector2){ i, c->current.rpm / 100.0 };
        throttle_line->points[i] = (Vector2){ i, c->input.throttle * 10 };
        run_pid(c, p, 0.1);
    }
}

void screen_update(graph* g, pid* p, int sim_time, double* delta){
    BeginDrawing();
        ClearBackground(DARKGRAY);

        *delta = GetFrameTime();

        graph_draw_border(g);
        graph_draw_grid(g);

        graph_draw_axis(g, X_AXIS, 0, DARKBLUE);
        graph_draw_axis(g, X_AXIS, sim_time, DARKBLUE);
        graph_draw_axis(g, Y_AXIS, 0, DARKBLUE);
        graph_draw_axis(g, Y_AXIS, p->target_speed, RED);

        graph_draw_lines(g);

        if (IsKeyDown(KEY_TAB)) {
            Vector2 m_pos = GetMousePosition();
            // normalize mouse position to graph
            m_pos.x -= g->margin + g->pos_x;
            m_pos.y = g->margin + g->height - m_pos.y - g->pos_y;
            m_pos = Vector2Scale(m_pos, 1.0 / g->scale);
            m_pos.x *= 1.0 / g->scale_x;
            m_pos.y *= 1.0 / g->scale_y;
            graph_draw_line_value_at_x(g, m_pos.x);
        }
        graph_draw_bottom_pane(g);
    EndDrawing();
}

char* shift_args(int* argc, char*** argv){
    if (*argc <= 0) return NULL;
    (*argc)--;
    return *((*argv)++);
}

int main(int argc, char** argv){
    SetTargetFPS(144);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(WIDTH, HEIGHT, "PID");

    pid* p = calloc(1, sizeof(pid));
    p->target_speed = 0.0;

    p->P = 0.1;
    p->I = 0.001;
    p->D = 0.1;

    double delta;

    char* selected_car = "corvette_c5";
    int graph_margin = 20;
    int sim_time = 760;

    char* program_name = shift_args(&argc, &argv);
    printf("%s\n", program_name);

    char* command;
    char* arg;

    while ((command = shift_args(&argc, &argv))) {
        if (!strcmp(command, "--speed")) {
            arg = shift_args(&argc, &argv);
            p->target_speed = atof(arg);
        } else if (!strcmp(command, "--car")) {
            arg = shift_args(&argc, &argv);
            selected_car = arg;
        } else if (!strcmp(command, "--graph-margin")) {
            arg = shift_args(&argc, &argv);
            graph_margin = atoi(arg);
        }
    }

    graph* g = get_graph(
        graph_margin, WIDTH, HEIGHT, BLACK, BLUE,
        "Target Speed", &p->target_speed,
        "Proportional", &p->P,
        "Integral", &p->I,
        "Derivative", &p->D
    );

    car* c = get_car(selected_car);
    g->car_name = c->name;

    line* speed_line = get_line(g, sim_time, "Speed (kmh)", PURPLE);
    line* rpm_line = get_line(g, sim_time, "RPM / 100.0", YELLOW);
    line* throttle_line = get_line(g, sim_time, "Throttle * 10", GREEN);
    line* gear_line = get_line(g, sim_time, "Gear * 10", BLUE);

    Vector2 mouse_last_pressed;
    bool mouse_pressed = false;
    bool tab_pressed = false;
    int should_draw_screen;

    run_sim(c, p, sim_time, speed_line, rpm_line, gear_line, throttle_line);
    screen_update(g, p, sim_time, &delta);

    while (!WindowShouldClose() && !IsKeyPressed(KEY_ENTER)) {
        should_draw_screen = 4;

        double zoom_speed = GetMouseWheelMove();
        if (zoom_speed != 0) {
            zoom_speed *= IsKeyDown(KEY_LEFT_SHIFT) ? 25.0 * g->scale : 5.0;
            graph_zoom(g, zoom_speed, delta);
            should_draw_screen += 1;
        }

        if (!mouse_pressed && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            mouse_last_pressed = GetMousePosition();
            mouse_pressed = true;
            should_draw_screen += 1;
        } else if (mouse_pressed) {
            double dx = GetMouseX() - mouse_last_pressed.x;
            double dy = GetMouseY() - mouse_last_pressed.y;

            g->pos_y -= dy * 2.5;
            g->pos_x += dx * 2.5;

            mouse_pressed = false;
            should_draw_screen += 1;
        }

        if (IsKeyPressed(KEY_SPACE)) {
            g->scale_x = 1;
            g->scale_y = 1;
            g->scale = 1;
            g->pos_y = 0;
            g->pos_x = 0;
            should_draw_screen += 1;
        }

        if (IsKeyDown(KEY_TAB)) {
            should_draw_screen += 1;
            tab_pressed = true;
        } else if(tab_pressed) {
            should_draw_screen += 1;
            tab_pressed = false;
        }

        if (IsKeyDown(KEY_J)) {
            p->target_speed += 50 * delta;
        } else if (IsKeyDown(KEY_K)) {
            p->target_speed -= 50 * delta;
            if (p->target_speed < 0) {
                p->target_speed = 0;
            }
        } else if (IsKeyDown(KEY_ONE)) {
            p->target_speed = round(p->target_speed);
        } else {
            should_draw_screen -= 1;
        }
        if (IsKeyDown(KEY_W)) {
            p->P += 0.01 * delta;
        } else if (IsKeyDown(KEY_S)) {
            p->P -= 0.01 * delta;
        } else if (IsKeyDown(KEY_X)) {
            p->P = 0;
        } else {
            should_draw_screen -= 1;
        }
        if (IsKeyDown(KEY_E)) {
            p->I += 0.01 * delta;
        } else if (IsKeyDown(KEY_D)) {
            p->I -= 0.01 * delta;
        } else if (IsKeyDown(KEY_C)){
            p->I = 0;
        } else {
            should_draw_screen -= 1;
        }
        if (IsKeyDown(KEY_R)) {
            p->D += 0.01 * delta;
        } else if (IsKeyDown(KEY_F)) {
            p->D -= 0.01 * delta;
        } else if (IsKeyDown(KEY_V)){
            p->D = 0;
        } else {
            should_draw_screen -= 1;
        }
        if (should_draw_screen == 0) {
            BeginDrawing();
                graph_draw_bottom_pane(g);
            EndDrawing();
            continue;
        }
        run_sim(c, p, sim_time, speed_line, rpm_line, gear_line, throttle_line);
        screen_update(g, p, sim_time, &delta);
    }

    free(p);
    free_car(c);
    graph_free(g);
    CloseWindow();
    return 0;
}

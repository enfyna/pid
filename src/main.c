#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#include "raylib.h"
#include "graph.h"

#define HEIGHT 300 * 2
#define WIDTH  400 * 2

int sign(double num){
    return num > 0 ? 1 : -1;
}

void run_pid(graph* g, Vector2* res, Vector2* sec, double* target_speed, double start_acc, double k_p, double k_i, double k_d){
    double current_speed = 100.0;

    double current_acc = start_acc;
    double inc_acc = 1.0;
    double max_acc = 5.0;

    double total_time = g->width;

    double it = 0.0;

    double dt = 1.0;
    double p_err = 0;
    for (int x = 0; x < total_time; x++) {
        double err = target_speed[x] - current_speed;
        double p = k_p * err;

        double i = k_i * err * dt;
        it = it + i;

        double d = k_d * ( err - p_err ) / dt;

        // amount of pressure on gas pedal
        double amount = p + it + d;

        if (fabs(amount) > inc_acc){
            amount = inc_acc * sign(amount);
        }
        current_acc += amount;
        if (fabs(current_acc) > max_acc){
            current_acc = max_acc * sign(current_acc);
        }
        current_speed += current_acc;

        p_err = err;
        res[x] = (Vector2){ x, current_speed };
        sec[x] = (Vector2){ x, amount * 10 + 50};
    }
}

int main(int argc, char** argv){
    double target_speed = 200.0;
    double start_acc = 0.0;

    double delta;

    double k_p = 0.1;
    double k_i = 0.001;
    double k_d = 0.1;

    int graph_margin = 20;

    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "--speed")) {
            target_speed = atof(argv[++i]);
        } else if (!strcmp(argv[i], "--graph-margin")) {
            graph_margin = atoi(argv[++i]);
        }
    }

    graph* g = get_graph(
        graph_margin, WIDTH, HEIGHT, BLACK, BLUE,
        "Target Spd", &target_speed,
        "Start Acc", &start_acc,
        "P", &k_p,
        "I", &k_i,
        "D", &k_d
    );

    Vector2* pid_line = malloc(sizeof(Vector2) * g->width);
    Vector2* target_line = malloc(sizeof(Vector2) * g->width);
    Vector2* amount_line = malloc(sizeof(Vector2) * g->width);

    double *target_speeds = malloc(sizeof(double) * g->width);

    SetTargetFPS(144);

    Vector2 mouse_last_pressed;
    bool mouse_pressed = false;

    InitWindow(WIDTH, HEIGHT, "PID");
    while (!WindowShouldClose() && !IsKeyPressed(KEY_ENTER)) {
        BeginDrawing();
            ClearBackground(DARKGRAY);
            graph_draw_border(g);

            double zoom_speed = GetMouseWheelMove();
            if (zoom_speed != 0) {
                double fast_zoom = IsKeyDown(KEY_LEFT_SHIFT) ? 5.0 * g->scale : 1.0;
                if (IsKeyDown(KEY_LEFT_CONTROL)) {
                    g->scale_x += zoom_speed * delta * fast_zoom;
                } else if (IsKeyDown(KEY_LEFT_ALT)) {
                    g->scale_y += zoom_speed * delta * fast_zoom;
                } else {
                    g->scale += zoom_speed * delta * fast_zoom;
                }
                if (g->scale_y <= 0.01) {
                    g->scale_y = 0.01;
                }
                if (g->scale_x <= 0.01) {
                    g->scale_x = 0.01;
                }
                if (g->scale <= 0.01) {
                    g->scale = 0.01;
                }
            }

            if (!mouse_pressed && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                mouse_last_pressed = GetMousePosition();
                mouse_pressed = true;
            } else if (mouse_pressed) {
                double dx = GetMouseX() - mouse_last_pressed.x;
                double dy = GetMouseY() - mouse_last_pressed.y;

                g->pos_y -= dy;
                g->pos_x += dx;

                mouse_pressed = false;
            }

            if (IsKeyPressed(KEY_SPACE)) {
                g->scale_x = 1;
                g->scale_y = 1;
                g->scale = 1;
                g->pos_y = 0;
                g->pos_x = 0;
            }
            graph_draw_grid(g);
            
            graph_draw_axis(g, X_AXIS, 0, DARKBLUE);
            graph_draw_axis(g, Y_AXIS, 0, DARKBLUE);

            delta = GetFrameTime();

            if (IsKeyDown(KEY_J)) {
                target_speed += 50 * delta;
            } else if (IsKeyDown(KEY_K)) {
                target_speed -= 50 * delta;
            } else if (IsKeyDown(KEY_ONE)) {
                target_speed = round(target_speed);
            }
            if (IsKeyDown(KEY_Q)) {
                start_acc += 1 * delta;
            } else if (IsKeyDown(KEY_A)) {
                start_acc -= 1 * delta;
            } else if (IsKeyDown(KEY_Z)) {
                start_acc = 0;
            }
            if (IsKeyDown(KEY_W)) {
                k_p += 0.01 * delta;
            } else if (IsKeyDown(KEY_S)) {
                k_p -= 0.01 * delta;
            } else if (IsKeyDown(KEY_X)) {
                k_p = 0;
            }
            if (IsKeyDown(KEY_E)) {
                k_i += 0.01 * delta;
            } else if (IsKeyDown(KEY_D)) {
                k_i -= 0.01 * delta;
            } else if (IsKeyDown(KEY_C)){
                k_i = 0;
            }
            if (IsKeyDown(KEY_R)) {
                k_d += 0.01 * delta;
            } else if (IsKeyDown(KEY_F)) {
                k_d -= 0.01 * delta;
            } else if (IsKeyDown(KEY_V)){
                k_d = 0;
            }

            for (int i = 0; i < g->width; i++) {
                target_speeds[i] = target_speed;
            }

            run_pid(g, pid_line, amount_line, target_speeds, start_acc, k_p, k_i, k_d);
            create_h_line(target_line, target_speeds, g->width);

            graph_draw_line(g, target_line, RED);
            graph_draw_line(g, pid_line, WHITE);
            graph_draw_line(g, amount_line, GREEN);

            graph_draw_bottom_pane(g);
        EndDrawing();
    }
    free(g);
    free(target_line);
    free(pid_line);
    free(amount_line);
    CloseWindow();
    return 0;
}

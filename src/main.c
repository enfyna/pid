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

void run_pid(graph* g, Vector2* res, double* target_speed, double start_acc, double k_p, double k_i, double k_d){
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
    }
}

void create_line(graph* g, Vector2* line, double* val){
    for (int i = 0; i < g->width; i++) {
        line[i] = (Vector2){ i, val[i] };
    }
}

int main(int argc, char** argv){
    double target_speed = 200.0;
    double start_acc = 0.0;

    double delta;

    double k_p = 0.1;
    double k_i = 0.001;
    double k_d = 0.1;

    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "--speed")) {
            target_speed = atof(argv[++i]);
        }
    }

    graph* g = get_graph(20, WIDTH, HEIGHT, BLACK, BLUE);

    g->pane.names[0] = "Target Spd";
    g->pane.values[0] = &target_speed;

    g->pane.names[1] = "Start Acc";
    g->pane.values[1] = &start_acc;

    g->pane.names[2] = "P";
    g->pane.values[2] = &k_p;

    g->pane.names[3] = "I";
    g->pane.values[3] = &k_i;

    g->pane.names[4] = "D";
    g->pane.values[4] = &k_d;

    g->pane.section_count = 5;

    Vector2* pid_line = malloc(sizeof(Vector2) * g->width);
    Vector2* target_line = malloc(sizeof(Vector2) * g->width);

    double *target_speeds = malloc(sizeof(double) * g->width);
    for (int i = 0; i < g->width; i++) {
        target_speeds[i] = target_speed;
    }

    SetTargetFPS(24);

    InitWindow(WIDTH, HEIGHT, "PID");
    while (!WindowShouldClose() && !IsKeyPressed(KEY_ENTER)) {
        BeginDrawing();
            ClearBackground(DARKGRAY);
            draw_graph_border(g);

            delta = GetFrameTime();

            if (IsKeyDown(KEY_J)) {
                target_speed += 50 * delta;
            } else if (IsKeyDown(KEY_K)) {
                target_speed -= 50 * delta;
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

            run_pid(g, pid_line, target_speeds, start_acc, k_p, k_i, k_d);
            create_line(g, target_line, target_speeds);

            draw_to_graph(g, target_line, RED);
            draw_to_graph(g, pid_line, WHITE);

            draw_bottom_pane(g);
        EndDrawing();
    }
    free(g);
    free(target_line);
    free(pid_line);
    CloseWindow();
    return 0;
}

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

void run_pid(Vector2* res, double* target_speed){
    double current_speed = 100.0;

    double current_acc = -5.0;
    double inc_acc = 1.0;
    double max_acc = 5.0;

    double total_time = WIDTH;

    double k_p = 0.1;
    double k_i = 0.001;
    double k_d = 0.1;

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

void create_line(Vector2* line, double* val){
    for (int i = 0; i < WIDTH; i++) {
        line[i] = (Vector2){ i, val[i] };
    }
}

int main(int argc, char** argv){
    double target_speed = 200.0;

    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "--speed")) {
            target_speed = atof(argv[++i]);
        }
    }

    double *target_speed_line = malloc(sizeof(double) * WIDTH);
    for (int i = 0; i < WIDTH; i++) {
        target_speed_line[i] = target_speed;
    }

    graph* g = get_graph(20, WIDTH, HEIGHT, BLACK, BLUE);

    Vector2* graph = malloc(sizeof(Vector2) * WIDTH);
    Vector2* line = malloc(sizeof(Vector2) * WIDTH);

    run_pid(graph, target_speed_line);
    create_line(line, target_speed_line);

    SetTargetFPS(24);

    InitWindow(WIDTH, HEIGHT, "PID");
    while (!WindowShouldClose() && !IsKeyPressed(KEY_ENTER)) {
        BeginDrawing();
            ClearBackground(DARKGRAY);
            draw_graph_border(g);
            draw_to_graph(g, line, RED);
            draw_to_graph(g, graph, WHITE);
        EndDrawing();
    }
    free(g);
    free(line);
    free(graph);
    CloseWindow();
    return 0;
}

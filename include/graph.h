#include "raylib.h"

#define get_graph(...) get_graph_null(__VA_ARGS__, NULL)

#define print_double(var) printf("%s: %lf\n", #var, var)
#define print_int(var) printf("%s: %d\n", #var, var)

#define GRID_MARGIN 50

typedef struct {
    int pos_x;
    int pos_y;
    int width;
    int height;
    int section_count;
    char* names[5];
    double* values[5];
} bottom_pane;

typedef struct {
    double pos_x;
    double pos_y;
    int margin;
    int width;
    int height;
    int w_width;
    int w_height;
    Color color;
    Color border_color;
    bottom_pane pane;
} graph;

graph* get_graph_null(int margin, int width, int height, Color color, Color border_color, ...);
void graph_draw_axis(graph* g, int type, int offset, Color color);
void graph_draw_line(graph* g, Vector2* line, Color color);
void graph_draw_border(graph* g);
void graph_draw_bottom_pane(graph* g);
void graph_draw_grid(graph *g);
void create_h_line(Vector2* line, double* val, int count);

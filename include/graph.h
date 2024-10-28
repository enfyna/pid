#include "raylib.h"

#define get_graph(...) get_graph_null(__VA_ARGS__, NULL)

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
void draw_axis_to_graph(graph* g, int type, int offset, Color color);
void draw_to_graph(graph* g, Vector2* line, Color color);
void create_line(graph* g, Vector2* line, double* val);
void draw_graph_border(graph* g);
void draw_bottom_pane(graph* g);

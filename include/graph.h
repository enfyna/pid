#include "raylib.h"

typedef struct {
    int margin;
    int width;
    int height;
    int w_width;
    int w_height;
    Color color;
    Color border_color;
} graph;

graph* get_graph(int margin, int width, int height, Color color, Color border_color);
void draw_to_graph(graph* g, Vector2* line, Color color);
void draw_graph_border(graph* g);

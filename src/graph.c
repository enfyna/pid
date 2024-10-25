#include <stdlib.h>

#include "graph.h"

void draw_to_graph(graph* g, Vector2* line, Color color){
    Vector2 prev_point;

    bool continuous = true;

    for (int x = 0; x < g->width; x++) {
        Vector2 point = line[x];
        point.x += g->margin;
        point.y = g->height - point.y;

        if (point.x <= g->margin) {
            continuous = false;
            continue;
        }
        else if (point.x >= g->width + g->margin) {
            continuous = false;
            continue;
        }

        if (point.y <= g->margin) {
            continuous = false;
            continue;
        }
        else if (point.y >= g->height + g->margin) {
            continuous = false;
            continue;
        }

        if (continuous && x > 0) {
            DrawLine(
                point.x, point.y,
                prev_point.x, prev_point.y,
                color
            );
        } else {
            continuous = true;
        }
        prev_point = point;
    }
}

void draw_graph_border(graph* g){
    DrawRectangle(
        g->margin, g->margin,
        g->width, g->height,
        g->color
    );
    DrawLine(
        g->margin, g->margin,
        g->margin, g->height + g->margin,
        g->border_color
    );
    DrawLine(
        g->margin, g->height + g->margin,
        g->width + g->margin, g->height + g->margin,
        g->border_color
    );
    DrawLine(
        g->margin, g->margin,
        g->width + g->margin, g->margin,
        g->border_color
    );
    DrawLine(
        g->width + g->margin, g->margin,
        g->width + g->margin, g->height + g->margin,
        g->border_color
    );
}

graph* get_graph(int margin, int width, int height, Color color, Color border_color){
    graph* g = malloc(sizeof(graph));

    g->border_color = border_color;
    g->color = color;

    g->margin = margin;

    g->height = height - margin * 6;
    g->width = width - margin * 2;

    g->w_width = width;
    g->w_height = height;

    return g;
}


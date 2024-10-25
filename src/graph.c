#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
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

void draw_bottom_pane(graph* g){
    if (g->pane.section_count <= 0) {
        return;
    }
    int section_width = g->pane.width / g->pane.section_count;
    for (int i = 0; i < g->pane.section_count; i++) {
        DrawText(
            TextFormat("%s:\n\n%f", g->pane.names[i], *g->pane.values[i]),
            g->pane.pos_x + section_width * i, g->pane.pos_y,
            24,
            WHITE
        );
        DrawLine(
            g->pane.pos_x + section_width * i, g->pane.pos_y,
            g->pane.pos_x + section_width * i, g->pane.pos_y + g->pane.height,
            g->border_color
        );
    }

}

void draw_graph_border(graph* g){
    // graph
    DrawRectangle(
        g->margin, g->margin,
        g->width, g->height,
        g->color
    );
    // right
    DrawLine(
        g->margin, g->margin,
        g->margin, g->height + g->margin,
        g->border_color
    );
    // bottom
    DrawLine(
        g->margin, g->height + g->margin,
        g->width + g->margin, g->height + g->margin,
        g->border_color
    );
    // top
    DrawLine(
        g->margin, g->margin,
        g->width + g->margin, g->margin,
        g->border_color
    );
    // left
    DrawLine(
        g->width + g->margin, g->margin,
        g->width + g->margin, g->height + g->margin,
        g->border_color
    );

    // bottom pane
    DrawRectangle(
        g->pane.pos_x, g->pane.pos_y,
        g->width, g->pane.height,
        g->color
    );
    // bottom
    DrawLine(
        g->pane.pos_x, g->pane.pos_y + g->pane.height,
        g->pane.pos_x + g->pane.width, g->pane.pos_y + g->pane.height,
        g->border_color
    );
    // top
    DrawLine(
        g->pane.pos_x, g->pane.pos_y,
        g->pane.pos_x + g->pane.width, g->pane.pos_y,
        g->border_color
    );
    // right
    DrawLine(
        g->pane.pos_x + g->pane.width, g->pane.pos_y,
        g->pane.pos_x + g->pane.width, g->pane.pos_y + g->pane.height,
        g->border_color
    );
    // left
    DrawLine(
        g->pane.pos_x, g->pane.pos_y,
        g->pane.pos_x, g->pane.pos_y + g->pane.height,
        g->border_color
    );
}

graph* get_graph(int margin, int width, int height, Color color, Color border_color){
    graph* g = malloc(sizeof(graph));

    g->border_color = border_color;
    g->color = color;

    g->margin = margin;

    g->height = height - g->margin * 6;
    g->width = width - g->margin * 2;

    g->w_width = width;
    g->w_height = height;

    g->pane.pos_x = g->margin;
    g->pane.pos_y = g->height + g->margin * 2;
    g->pane.width = g->width;
    g->pane.height = g->margin * 3;
    g->pane.section_count = 0;

    return g;
}


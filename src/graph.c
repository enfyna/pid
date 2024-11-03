#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"
#include "graph.h"

static void _graph_draw_line(graph* g, line* l);

void create_h_line(Vector2* line, double* val, int count){
    for (int i = 0; i < count; i++) {
        line[i] = (Vector2){ i, val[i] };
    }
}

void create_v_line(Vector2* line, double* val, int count){
    for (int i = 0; i < count; i++) {
        line[i] = (Vector2){ i, val[i] };
    }
}

void graph_draw_axis(graph* g, int type, int offset, Color color){
    line axis = {
        .color = color,
        .count = 2,
        .points = malloc(sizeof(Vector2) * 2),
    };
    if (type == Y_AXIS) {
        double px  = -g->pos_x / (g->scale * g->scale_x);
        double len = g->width / (g->scale * g->scale_x);
        axis.points[0] = (Vector2){ px + 1, offset };
        axis.points[1] = (Vector2){ px + len - 1, offset };
    } else {
        double py = -g->pos_y / (g->scale * g->scale_y);
        double len = g->height / (g->scale * g->scale_y);
        axis.points[0] = (Vector2){ offset, py + 1 };
        axis.points[1] = (Vector2){ offset, py + len - 1 };
    }
    _graph_draw_line(g, &axis);
    free(axis.points);
}

void graph_draw_relative_line(graph* g, int type, int offset, Color color){
    int y = g->margin + g->height;
    if (type == X_AXIS) {
        DrawLine(
            g->margin, y - offset,
            g->margin + g->width, y - offset,
            color
        );
    } else if (type == Y_AXIS) {
        DrawLine(
            g->margin + offset, g->margin,
            g->margin + offset, g->margin + g->height,
            color
        );
    }
}

void graph_draw_line_with_name(graph* g, Vector2* points, const char* name, Color color){
    DrawRectangle(g->margin + 10, g->margin + 5 + 25 * g->line_count, 20, 20, color);
    DrawText(name, g->margin + 40, g->margin + 5 + 25 * g->line_count, 20, color);
    g->line_count++;

    line l = {
        .points = points,
        .count = g->width,
        .color = color,
    };

    _graph_draw_line(g, &l);
}

static void _graph_draw_line(graph* g, line* line){
    Vector2 prev_point;

    bool continuous = true;

    for (int x = 0; x < line->count; x++) {
        Vector2 point = Vector2Scale(line->points[x], g->scale);
        point.x *= g->scale_x;
        point.y *= g->scale_y;

        point.x += g->margin + g->pos_x;
        point.y = g->margin + g->height - point.y - g->pos_y;

        if (point.x < g->margin) {
            continuous = false;
            continue;
        }
        else if (point.x > g->width + g->margin) {
            continuous = false;
            continue;
        }

        if (point.y < g->margin) {
            continuous = false;
            continue;
        }
        else if (point.y > g->height + g->margin) {
            continuous = false;
            continue;
        }

        if (continuous && x > 0) {
            DrawLineEx(
                point,
                prev_point,
                2, line->color
            );
        } else {
            continuous = true;
        }
        prev_point = point;
    }
}

void graph_draw_grid(graph *g){
    double margin = GRID_MARGIN * g->scale;
    for (double x = fmod(g->pos_y, margin * g->scale_y); x < g->height; x+=margin * g->scale_y) {
        graph_draw_relative_line(g, 0, x, DARKGRAY);
    }
    for (double y = fmod(g->pos_x, margin * g->scale_x); y < g->width; y+=margin * g->scale_x) {
        graph_draw_relative_line(g, 1, y, DARKGRAY);
    }
}

void graph_draw_bottom_pane(graph* g){
    DrawText(
        TextFormat("%%%.2lf", g->scale),
        g->margin + g->width - 75, g->margin + g->height - 25, 
        24, WHITE
    );

    if (g->pane.section_count <= 0 || g->pane.height == -1) {
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

void graph_draw_border(graph* g){
    g->line_count = 0;
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

    if (g->pane.height == -1) {
        return;
    }
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

graph* get_graph_null(int margin, int width, int height, Color color, Color border_color, ...){
    graph* g = malloc(sizeof(graph));

    va_list ap;
    va_start(ap, border_color);

    char *title;
    double *value;

    while ((title = va_arg(ap, char*)) && (value = va_arg(ap, double *))) {
        g->pane.names[g->pane.section_count] = title;
        g->pane.values[g->pane.section_count] = value;

        g->pane.section_count++;
    }
    va_end(ap);

    g->pos_x = 0;
    g->pos_y = 0;
    g->scale = 1.0;
    g->scale_x = 1.0;
    g->scale_y = 1.0;

    g->border_color = border_color;
    g->color = color;

    g->margin = margin;

    g->w_width = width;
    g->w_height = height;

    if (g->pane.section_count > 0 && g->margin * 3 >= 60) {
        g->height = height - g->margin * 6;
        g->width = width - g->margin * 2;

        g->pane.pos_x = g->margin;
        g->pane.pos_y = g->height + g->margin * 2;
        g->pane.width = g->width;
        g->pane.height = g->margin * 3;
    } else {
        g->height = height - g->margin * 2;
        g->width = width - g->margin * 2;

        g->pane.pos_x = -1;
        g->pane.pos_y = -1;
        g->pane.width = -1;
        g->pane.height = -1;
        return g;
    }
    return g;
}


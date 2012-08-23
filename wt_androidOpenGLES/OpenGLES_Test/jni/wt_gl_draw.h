#ifndef _WT_GL_H_
#define _WT_GL_H_
#include "wt_android_core.h"

static const GLfloat wt_cir_data[] =
{
    0.0000f,  1.0000f, 0.2588f,  0.9659f,
    0.5000f,  0.8660f, 0.7071f,  0.7071f,
    0.8660f,  0.5000f, 0.9659f,  0.2588f,
    1.0000f,  0.0000f, 0.9659f, -0.2588f,
    0.8660f, -0.5000f, 0.7071f, -0.7071f,
    0.5000f, -0.8660f, 0.2588f, -0.9659f,
    0.0000f, -1.0000f, -0.2588f, -0.9659f,
    -0.5000f, -0.8660f, -0.7071f, -0.7071f,
    -0.8660f, -0.5000f, -0.9659f, -0.2588f,
    -1.0000f, -0.0000f, -0.9659f,  0.2588f,
    -0.8660f,  0.5000f, -0.7071f,  0.7071f,
    -0.5000f,  0.8660f, -0.2588f,  0.9659f,
    0.0000f,  1.0000f,  0.0f, 0.0f
};

static const GLfloat wt_rect_data[] = {
        -1.0, 1.0,           // Top left
        -1.0, -1.0,           // Bottom left
        1.0, -1.0,           // Bottom right
        1.0, 1.0,          // Top right
};
static const int wt_cir_count = sizeof(wt_cir_data) / sizeof(GLfloat) / 2;

typedef struct
{
    wt_r32 r, g, b;
} wt_gl_color;

void wt_draw_cir(wt_circle cir, wt_gl_color c);
void wt_draw(wt_world *w);
void wt_draw_shapes(wt_array *shapes);

#endif
#ifndef WT_GL_H
#define WT_GL_H
#include "Head/glut.h"
#include "Head/glaux.h" 
#include "../wt_core/wt_core.h"

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
static const int wt_cir_count = sizeof(wt_cir_data) / sizeof(GLfloat) / 2;

static const GLfloat wt_rect_data[] = {
        0.0, 1.0,
        1.0, 1.0,
        0.0, 0.0,
        1.0, 0.0 
            
};


static const int wt_rect_count = sizeof(wt_rect_data) / sizeof(GLfloat) / 2;

typedef struct
{
    wt_r32 r, g, b;
} wt_gl_color;


void wt_gl_main();
void wt_draw_cir(wt_circle cir, wt_gl_color c);
void wt_draw_dot2f(float x, float y);
void wt_draw_dot(wt_vec p, wt_r32 size, wt_gl_color c);
//void wt_draw_rect(wt_rect rect, wt_gl_color c);
#endif
#ifndef _WT_DEMO_H_
#define _WT_DEMO_H_

#include "wt_core.h"

static void wt_cir_wall_bottom(wt_world *w)
{
    wt_r32 r = 3;
    for (wt_r32 i = 0; i <= w->width + 15 ; i += 2 * r)
    {
        wt_body *b3 = wt_create_body0(WT_MAX_R32, wt_v(i, r), 10.0);
        b3->fric = 0.8 ;
        b3->restitution = 0.2;
        wt_circle *c3 = wt_create_cir(b3, r);
        wt_shape *s3 = wt_create_shape(c3, WT_CIR);
        wt_world_add_shape(w, s3);
    }
}

static void wt_circle_pyramid(wt_world *w)
{
    wt_r32 gap = 2;
    int num = 10;
    wt_r32 r = 3;
    for (wt_r32 i = 0 ; i < num ; i++)
    {
        for (wt_r32 j = 0 ; j <= i ; j++)
        {
            wt_r32 x = w->width / 2 - i * r;
            wt_r32 y = w->width - w->width / 10 - i * 2 * r ;
            x += j * 2 * r;
            wt_body *b3 = wt_create_body0(500, wt_v(x, y), 10.0);
            b3->fric = 0.8 ;
            b3->restitution = 0.2;
            b3->ael.y = -40;
            wt_circle *c3 = wt_create_cir(b3, r);
            wt_shape *s3 = wt_create_shape(c3, WT_CIR);
            wt_world_add_shape(w, s3);
        }
    }
}

static void wt_circle_matrix(wt_world *w)
{
    wt_r32 gap = 2;
    int num = 35;
    wt_r32 r = 1;
    for (wt_r32 i = 0 ; i < num ; i++)
    {
        for (wt_r32 j = 0 ; j < num ; j++)
        {
            wt_r32 x = w->width / 2 - num * r + j * 2 * r;
            wt_r32 y = w->width  - 10 - i * 2 * r ;
            //x += j * 2 * r;
            wt_body *b3 = wt_create_body0(10, wt_v(x, y), 10.0);
            b3->fric = 0.8 ;
            b3->restitution = 0.2;
            b3->ael.y = -100;
            wt_circle *c3 = wt_create_cir(b3, r);
            wt_shape *s3 = wt_create_shape(c3, WT_CIR);
            wt_world_add_shape(w, s3);
        }
    }
}

static void wt_generate_fluid_partical(wt_world *w, float x, float y, float r)
{
    //wt_body *b = wt_create_partical(10, r, wt_v(x, y), wt_v(0, 0), wt_v(0, -10));
    wt_body *b = wt_create_body0(1, wt_v(x, y), 0);
    wt_pvf_partical *pvf_p = wt_create_pvf_partical(b);
    wt_pvf_add_partical(w->fluid, pvf_p);
}

static void wt_generate_circle(wt_world *w, float x, float y, float r)
{
    wt_body *b3 = wt_create_body0(WT_MAX_R32, wt_v(x, y), 10.0);
    b3->fric = 0.8 ;
    b3->restitution = 0.2;
    //b3->ael.y = ;
    wt_circle *c3 = wt_create_cir(b3, r);
    wt_shape *s3 = wt_create_shape(c3, WT_CIR);
    wt_world_add_shape(w, s3);
}

#endif
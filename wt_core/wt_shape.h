#ifndef _WT_SHAPE_H_
#define _WT_SHAPE_H_

#include "wt_core.h"

#define WT_CIR 1

typedef struct 
{
	wt_vec pos_tl;
	wt_vec pos_br;
} wt_AABB;

typedef struct {
	void *shape;
	wt_i32 type;
} wt_shape;

typedef struct
{
    wt_body *body;
    wt_r32 radius;
    //圆形转动惯量  MR^2/2
} wt_circle;

wt_circle *wt_create_cir(wt_body *body, wt_r32 radius);
wt_shape *wt_create_shape(void *shape, wt_i32 type);
wt_body *wt_shape_get_body(wt_shape *shape);
wt_AABB wt_shape_get_AABB(wt_shape *shape);
wt_r32 wt_shape_get_around_cir(wt_shape *shape);
#endif
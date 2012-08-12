#ifndef _WT_PARTICAL_H_
#define _WT_PARTICAL_H_

#include "wt_core.h"
//用于模拟流体粒子
typedef struct
{
    wt_r32 mas;
    wt_r32 inv_mas;
    wt_r32 radius;//用于绘图

    wt_vec pos;
    wt_vec pre_pos;
    wt_vec vel;
    wt_vec ael;

    //wt_r32 max_vel;
} wt_partical;

wt_partical *wt_create_partical(wt_r32 mas,wt_r32 radius, wt_vec pos, wt_vec vel, wt_vec ael);
wt_status    wt_partical_update(wt_partical *p, wt_r32 dt);
void wt_partical_restrict_vel(wt_partical *pi, wt_r32 max_vel);
void wt_partical_collide_border(wt_partical *pi);
#endif
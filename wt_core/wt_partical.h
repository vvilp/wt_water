#ifndef _WT_PARTICAL_H_
#define _WT_PARTICAL_H_

#include "wt_core.h"
//用于模拟流体粒子
typedef struct
{
    wt_r32 mas;
    wt_r32 inv_mas;

    wt_vec pos;
    wt_vec vel;
    wt_vec ael;
} wt_partical;

wt_partical *wt_create_partical(wt_r32 mas, wt_vec pos, wt_vec vel, wt_vec ael);
void         wt_partical_update(wt_partical *p, wt_r32 dt);
#endif

#ifndef _WT_WORLD_H
#define _WT_WORLD_H
#include "../wt_common/wt_common.h"
#include "wt_all.h"
#include "wt_shape.h"

wt_world *wt_create_world();
wt_status wt_world_add_shape(wt_world *w, wt_shape *s);
wt_status wt_world_update_per_step(wt_world *w);
wt_status wt_world_update_step(wt_world *w, wt_r32 wt_time);
wt_status wt_world_add_spring(wt_world *w, wt_spring *s);


static wt_status wt_spring_update_body_force(wt_spring *spring)
{
    wt_r32 change_len = wt_vlen(wt_vsub(spring->b2->pos, spring->b1->pos)) - spring->relaxlen ;
    //if(change_len >  spring->maxlen)  change_len = spring->maxlen;
    //if(change_len < -spring->maxlen)  change_len = -spring->maxlen;

    //b1 指向 b2 的单位向量
    wt_vec unit_b1b2 = wt_vunit(wt_vsub(spring->b2->pos, spring->b1->pos));
    //弹力
    wt_r32 f = change_len * spring->coef;
    spring->b1->force = wt_vmuls(unit_b1b2, f);
    spring->b2->force = wt_vmuls(unit_b1b2, -f);

    spring -> b1->force = wt_vsub(spring->b1->force, wt_vmuls(spring->b1->vel, spring->damp));
    spring -> b2->force = wt_vsub(spring->b2->force, wt_vmuls(spring->b2->vel, spring->damp));

    wt_r32 pe = change_len * change_len * spring->coef / 2; //弹性势能
    wt_r32 ve = wt_body_energy(spring->b1) + wt_body_energy(spring->b2);

    printf("能量: %f\n", pe + ve);

}

static wt_status wt_world_update_springs(wt_array *springs)
{
    //wt_array* springs = springs;
    for (int i = 0 ; i < springs->num ; i++)
    {
        wt_spring *spring = springs->array[i];
        wt_spring_update_body_force(spring);
    }

}

static wt_status wt_world_update_bodys(wt_world *w, float wt_time)
{
    wt_array *ss = w -> shapes;
    for (int i = 0; i < ss -> num; ++i)
    {
        wt_shape *s = ss -> array[i];
        wt_body  *b = wt_shape_get_body(s);
        wt_body_update_step(b, wt_time);
    }
}

static wt_status wt_world_update_liquid(wt_world *w, float wt_time)
{
    wt_array *l = w->liquid;
    for (int i = 0 ; i < l->num; i++)
    {
        wt_liquid *liquid = l->array[i];
        wt_world_update_springs(liquid->springs);

        wt_array *bodys = liquid->bodys;
        for (int j = 0 ; j < bodys->num; j++)
        {
            wt_body  *b = bodys->array[j];
            wt_body_update_step(b, wt_time);
        }
    }

}

#endif

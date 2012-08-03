#include "wt_core.h"

wt_world *w = NULL;


wt_world *wt_create_world()
{
    wt_world *w = NULL;
    w = (wt_world *) malloc (sizeof(wt_world));
    w -> shapes   = wt_array_init(100);
    w -> contacts = wt_array_init(100);
    w -> gravity  = wt_v(0, 0);
    w -> fluid = wt_create_sph_fluid(1000.0, 1.0, 1.0,4);
    return w;
}


void wt_world_int()
{
    wt_info("wt_world_int \n", 1);
    w = wt_create_world();
}

void wt_world_run()
{
    //wt_info("wt_world_Run \n", 1);
    wt_world_step(1.0 / 60.0);
}

wt_world *wt_get_world()
{
    return w;
}

wt_status wt_world_add_shape(wt_world *w, wt_shape *s)
{
    if (wt_array_add(w->shapes, s) == WT_OK)
        return WT_OK;
    else
        return WT_ER;
}

//临时
wt_status wt_update_collide_border(wt_world *w)
{
    wt_array *ss = w->shapes;
    for (int i = 0; i < ss->num; ++i)
    {
        wt_shape *s = ss->array[i];
        wt_circle *c = s->shape;
        wt_body *b = c->body;
        if (b->pos.y >= 100)
            b->vel.y = -b->vel.y;
        if (b->pos.y <= -100)
            b->vel.y = -b->vel.y;
        if (b->pos.x >= 100)
            b->vel.x = -b->vel.x;
        if (b->pos.x <= -100)
            b->vel.x = -b->vel.x;
    }
}



wt_status wt_world_update_bodys(wt_world *w, float wt_time)
{
    wt_array *ss = w -> shapes;
    for (int i = 0; i < ss -> num; ++i)
    {
        wt_shape *s = ss -> array[i];
        wt_body  *b = wt_shape_get_body(s);
        wt_body_update_step(b, wt_time);
    }
}

void wt_world_set_gravity(wt_world *w, wt_vec gravity)
{
    w->gravity = gravity;
}

void wt_world_update_fluid(wt_world *w,wt_r32 dt)
{
    wt_sph_update_fluid(w->fluid,dt);

}

void wt_world_step(wt_r32 dt)
{
    wt_collision_detect(w);

    wt_collision_before_solve(w, dt);

    wt_collision_solve(w, dt);

    wt_update_collide_border(w);

    wt_world_update_bodys(w, dt);

    wt_world_update_fluid(w, dt);
}
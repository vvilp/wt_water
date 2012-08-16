#include "wt_core.h"

wt_world *w = NULL;


wt_world *wt_create_world()
{
    wt_world *w = NULL;
    w = (wt_world *) malloc (sizeof(wt_world));
    w -> shapes   = wt_array_init(50);
    w -> contacts = wt_array_init(100);
    w -> gravity  = wt_v(0, 0);
    w -> fluid    = wt_create_pvf_fluid();
    //w -> hash     = wt_init_spatial_hash(400,5);
    w -> width      = 100;
    w -> shapes_table = wt_create_spatial_table(w->width, 1);
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
    wt_world_step(0.02);
}

wt_world *wt_get_world()
{
    return w;
}

void wt_shape_table_add_shape(wt_spatial_table *table, wt_shape *s, int is_table_only)
{
    //wt_debug("wt_shape_table_add_shape\n", 1);
    wt_r32 radius = 0;
    wt_body *body = wt_shape_get_body(s);
    if(s->type == WT_CIR){
        wt_circle *c = (wt_circle *)s->shape;
        radius = c->radius;
    }
    if(is_table_only){
        wt_spatial_table_add_obj_table_only(w->shapes_table,s,body->pos.x,body->pos.y,radius);
    }else{
        wt_spatial_table_add_obj(w->shapes_table,s,body->pos.x,body->pos.y,radius);
    }
    
}

void wt_shape_table_reset(wt_spatial_table *table)
{
    for(int i = 0 ; i < table->cell_num ; i++)
    {
        for(int j = 0 ; j < table->cell_num ; j++){
            wt_array_clear(table->table[i][j]);
        }
    }
    wt_array *list = table->all_list;
    for(int i = 0 ; i < list->num ; i++){
        wt_shape *s = list->array[i];
        wt_shape_table_add_shape(table, s, 1);
    }
}


wt_status wt_world_add_shape(wt_world *w, wt_shape *s)
{
    //wt_debug("wt_world_add_shape\n", 1);
    wt_array_add(w->shapes, s);
    wt_shape_table_add_shape(w->shapes_table, s, 0);
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
        if (b->pos.y >= w->width - c->radius)
            b->vel.y = -b->vel.y;
        if (b->pos.y <= 0 + c->radius)
            b->vel.y = -b->vel.y;
        if (b->pos.x >= w->width - c->radius)
            b->vel.x = -b->vel.x;
        if (b->pos.x <= 0 - c->radius)
            b->vel.x = -b->vel.x;
    }
}



wt_status wt_world_update_bodys(wt_world *w, float wt_time)
{

    wt_array *ss = w -> shapes;
    if(ss->num % 1000 == 0) {
        wt_debug("wt_shape num : %d \n ", ss->num);
    }
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
    //wt_sph_update_fluid(w->fluid,dt);
    wt_pvf_update_fluid(w->fluid,dt);
}

void wt_world_set_hash(wt_world *w)
{
    //system("pause");
    wt_array *ss = w -> shapes;
    for (int i = 0; i < ss -> num; ++i)
    {
        wt_shape *s = ss -> array[i];
        wt_body  *b = wt_shape_get_body(s);
        wt_AABB aabb = wt_shape_get_AABB(ss -> array[i]);
        //wt_debug("aabb : %f , %f \n", aabb.pos_tl.x, aabb.pos_tl.y);
        wt_add_to_spatial_hash(w->hash,aabb.pos_tl,aabb.pos_br,s);
    }
}



void wt_world_step(wt_r32 dt)
{
    //wt_clear_spatial_hash(w->hash);

    //system("pause");

    //wt_world_set_hash(w);
     //system("pause");

    wt_shape_table_reset(w->shapes_table);

    wt_collision_detect_table_version(w);

    //wt_collision_detect(w);

    wt_collision_before_solve(w, dt);

    wt_collision_solve(w, dt);

    wt_update_collide_border(w);

    wt_world_update_bodys(w, dt);

    wt_world_update_fluid(w, dt);
}
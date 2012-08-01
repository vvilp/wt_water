

#include "wt_core/wt_core.h"
#include "wt_android_h.h"
#include "wt_gl.h"
wt_world *world=NULL;
//=================================
//samples
void wt_cir_test()
{
   wt_body *b1 = wt_create_body0(10.0, wt_v(-20, 0), 0);
    b1->vel.x = 2.0;
    b1->fric = 0.8;
    b1->ang_vel = -1;
    b1->ael.y = -5.0;//b1->ael.y = 9.8;
    b1->restitution = 0.2;
    wt_circle *c1 = wt_create_cir(b1, 5.0);
    wt_shape *s1 = wt_create_shape(c1, WT_CIR);
    wt_world_add_shape(world, s1);

    wt_body *b2 = wt_create_body0(10, wt_v(0, 0), 0);
    wt_circle *c2 = wt_create_cir(b2, 8.0);
    wt_shape *s2 = wt_create_shape(c2, WT_CIR);
    b2->fric = 0.8 ;
    b2->ael.y = -5.0;
    b2->restitution = 0.2;
    wt_world_add_shape(world, s2);

}

void wt_2cir_collision_test()
{

   
}

void wt_cir_collision_test()
{

}


void wt_rect_test()
{
   
}
void wt_spring_test()
{
 
}

void wt_circle_pyramid(wt_world *w)
{
    wt_r32 gap = 2.0;
    int num = 10;
    wt_r32 before_line_first_x = 0;
    for (wt_r32 i = 0 ; i < num ; i++)
    {
        for (wt_r32 j = 0 ; j <= i ; j++)
        {
            wt_r32 r = 5.0;
            wt_r32 y = 90.0 - i * 2 * r - gap;
            wt_r32 x = before_line_first_x + j * 2 * r + gap;
            wt_body *b3 = wt_create_body0(10, wt_v(x, y), 10.0);
            b3->fric = 0.8 ;
            b3->restitution = 0.2;
            b3->ael.y = w->gravity.y;
            b3->ael.x = w->gravity.x;
            wt_circle *c3 = wt_create_cir(b3, r);
            wt_shape *s3 = wt_create_shape(c3, WT_CIR);
            wt_world_add_shape(w, s3);
        }
        before_line_first_x = before_line_first_x - 5;
    }

}

void wt_cir_wall(wt_world *w)
{
    for (wt_r32 i = -105.0; i <= 105.0 ; i += 10.0)
    {
        wt_body *b3 = wt_create_body0(WT_MAX_R32, wt_v(i, -90), 10.0);
        b3->fric = 0.8 ;
        b3->restitution = 0.2;
        wt_circle *c3 = wt_create_cir(b3, 5.0);
        wt_shape *s3 = wt_create_shape(c3, WT_CIR);
        wt_world_add_shape(w, s3);
    }
}
//========================================================================

void wt_draw_shapes(wt_array *shapes)
{
    wt_gl_color c;
    c.r = 1.0; c.g = 0.0; c.b = 0.0;

    //LOGI("wt_draw_shapes:%d", shapes->num);
    wt_circle *cir;
    for (int i = 0; i < shapes->num; ++i)
    {
        wt_shape *s = shapes->array[i];
        if (s->type == WT_CIR)
        {
            cir = (wt_circle *) (s->shape);
            
            wt_body *b = (wt_body *)cir->body;
            //LOGI("wt_draw_cir r:%f", cir->radius);
            //LOGI("wt_draw_cir pos x:%f , y:%f",b->pos.x, b->pos.y);
           //LOGI("wt_draw_cir vel x:%f , y:%f",b->vel.x, b->vel.y);
            wt_draw_cir(*cir, c);
            
        }
    }
    // for (int i = 0; i < shapes->num; ++i)
    // {
    //     wt_shape *s = shapes->array[i];
    //     if (s->type == WT_REC)
    //     {
    //         wt_rect *r = (wt_rect *) (s->shape);
    //         wt_draw_rect(*r, c);
    //     }
    // }

}


void wt_generate_body(wt_world *w)
{

    wt_circle_pyramid(w);
    wt_cir_wall(w);
}

void wt_draw()
{
    wt_array *shapes = world->shapes;
    wt_draw_shapes(shapes);
}

void initPhy()
{
    wt_world_int();
    world = wt_get_world();
    wt_generate_body(world);
    LOGI("INIT openGles");
}

void runPhy()
{
    wt_world_run();
    wt_draw();
}


void wt_setGravity(float x,float y)
{
    if(world!=NULL){
        //LOGI("setGravity x:%f  y:%f \n",x,y);
        world->gravity = wt_v(x,y);
    }else{
        LOGI("w = null ");
    }

}

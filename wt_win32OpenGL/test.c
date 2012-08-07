
#include "../wt_core/wt_core.h"
#include "wt_gl.h";

wt_world *w_world = NULL;



void wt_bullit_test(wt_world *w)
{
    // wt_body *b2 = wt_create_body0(10, wt_v(1, 90), 0);
    // wt_circle *c2 = wt_create_cir(b2, 1.0);
    // wt_shape *s2 = wt_create_shape(c2, WT_CIR);
    // b2->fric = 0.8 ;
    // b2->ael.y = -5.0;
    // b2->vel.y = -500;
    // b2->restitution = 0.2;
    // wt_world_add_shape(w, s2);

    // b2 = wt_create_body0(10, wt_v(-50, 90), 0);
    // c2 = wt_create_cir(b2, 1.0);
    // s2 = wt_create_shape(c2, WT_CIR);
    // b2->fric = 0.8 ;
    // b2->ael.y = -5.0;
    // b2->vel.y = -200;
    // b2->restitution = 0.2;
    // wt_world_add_shape(w, s2);

    // b2 = wt_create_body0(10, wt_v(50, 90), 0);
    // c2 = wt_create_cir(b2, 1.0);
    // s2 = wt_create_shape(c2, WT_CIR);
    // b2->fric = 0.8 ;
    // b2->ael.y = -5.0;
    // b2->vel.y = -200;
    // b2->restitution = 0.2;
    // wt_world_add_shape(w, s2);

    // wt_body *b3 = wt_create_body0(10, wt_v(0, 0), 10.0);
    // b3->fric = 0.8 ;
    // b3->restitution = 0.2;
    // b3->ael.y = -5.0;
    // wt_circle *c3 = wt_create_cir(b3, 1);
    // wt_shape *s3 = wt_create_shape(c3, WT_CIR);
    // wt_world_add_shape(w, s3);
    int num = 20;
    wt_r32 gap = 2;
    for (wt_r32 i = 0 ; i < num ; i++)
    {
        for (wt_r32 j = 0 ; j <= num ; j++)
        {
            wt_r32 r = 10.0;
            wt_r32 y = 550.0 - i * 2 * r - gap;
            wt_r32 x = num * r + j * 2 * r + gap;
            wt_body *b3 = wt_create_body0(10, wt_v(x, y), 10.0);
            b3->fric = 0.8 ;
            b3->restitution = 0.2;
            b3->ael.y = -6.8;
            wt_circle *c3 = wt_create_cir(b3, r);
            wt_shape *s3 = wt_create_shape(c3, WT_CIR);
            wt_world_add_shape(w, s3);
        }

    }
}

void wt_circle_test(wt_world *w)
{
    wt_body *b3 = wt_create_body0(10, wt_v(250, 250), 10.0);
    b3->fric = 0.8 ;
    b3->restitution = 0.2;
    b3->ael.y = -10;
    wt_circle *c3 = wt_create_cir(b3, 10);
    wt_shape *s3 = wt_create_shape(c3, WT_CIR);
    wt_world_add_shape(w, s3);
}
void wt_circle_pyramid(wt_world *w)
{
    wt_r32 gap = 2;
    int num = 10;
    for (wt_r32 i = 0 ; i < num ; i++)
    {
        for (wt_r32 j = 0 ; j <= i ; j++)
        {
            wt_r32 r = w->width / 50;
            wt_r32 y = w->width - w->width / 10 - i * 2 * r ;
            wt_r32 x = w->width/2 + j * 2 * r ;
            wt_body *b3 = wt_create_body0(10, wt_v(x, y), 10.0);
            b3->fric = 0.8 ;
            b3->restitution = 0.2;
            b3->ael.y = -6.8;
            wt_circle *c3 = wt_create_cir(b3, r);
            wt_shape *s3 = wt_create_shape(c3, WT_CIR);
            wt_world_add_shape(w, s3);
        }

    }
}

void wt_cir_wall(wt_world *w)
{
    for (wt_r32 i = 0; i <= w->width ; i += w->width / 20)
    {
        wt_body *b3 = wt_create_body0(WT_MAX_R32, wt_v(i, w->width / 40), 10.0);
        b3->fric = 0.8 ;
        b3->restitution = 0.2;
        wt_circle *c3 = wt_create_cir(b3, w->width / 40);
        wt_shape *s3 = wt_create_shape(c3, WT_CIR);
        wt_world_add_shape(w, s3);
    }
}

void wt_generate_body(wt_world *w)
{
    //wt_bullit_test(w);
    //wt_circle_test(w);
    wt_circle_pyramid(w);
    //wt_circle_pyramid(w);
    wt_cir_wall(w);
}

void wt_generate_fluid(wt_world *w)
{
    wt_partical *p = wt_create_partical(1, 20, wt_v(50,50), wt_v(0,0), wt_v(0,0));
    wt_pvf_partical *pvf_p = wt_create_pvf_partical(p);
    wt_pvf_add_partical(w->fluid, pvf_p);
}

void run()
{
    wt_world_run();
    wt_draw(w_world);
    _sleep(10);
}

void runPhy()
{
    wt_world_int();
    w_world = wt_get_world();
    //wt_generate_body(w_world);
    wt_generate_fluid(w_world);
    wt_gl_main(&run);

}

int main()
{
    runPhy();

}

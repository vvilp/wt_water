
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
    int num = 60;
    for (wt_r32 i = 0 ; i < num ; i++)
    {
        for (wt_r32 j = 0 ; j <= i ; j++)
        {
            wt_r32 r = 5.0;
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

void wt_cir_wall(wt_world *w)
{
    for (wt_r32 i = 0; i <= 600 ; i += 10.0)
    {
        wt_body *b3 = wt_create_body0(WT_MAX_R32, wt_v(i, 2), 10.0);
        b3->fric = 0.8 ;
        b3->restitution = 0.2;
        wt_circle *c3 = wt_create_cir(b3, 5.0);
        wt_shape *s3 = wt_create_shape(c3, WT_CIR);
        wt_world_add_shape(w, s3);
    }
}

void wt_generate_body(wt_world *w)
{
    wt_bullit_test(w);
    //wt_circle_test(w);
    //wt_circle_pyramid(w);
    //wt_circle_pyramid(w);
    wt_cir_wall(w);
}

void wt_generate_fluid(wt_world *w)
{
    // //w->fluid =
    // //wt_partical *p = wt_create_partical(10, 0.2, wt_v(0,0), wt_v(0,0), wt_v(0,-9.8));
    // wt_partical *p = wt_create_partical(10, 2, wt_v(0, 0), wt_v(0, 0), wt_v(0, -9.8));
    // wt_sph_partical *sp = wt_create_sph_partical(p);

    // wt_sph_add_partical(w->fluid, sp);

    // //p = wt_create_partical(10, 0.2, wt_v(0,0.5), wt_v(0,0), wt_v(0,-9.8));
    // p = wt_create_partical(10, 2, wt_v(0, 5), wt_v(0, 0), wt_v(0, 0));
    // sp = wt_create_sph_partical(p);

    // wt_sph_add_partical(w->fluid, sp);

    wt_r32 r = 0.2;

    for (int i = 0 ; i < 20 ; i++)
    {
        for (int j = 0 ; j < 20 ; j++)
        {
            wt_partical *p = wt_create_partical(10, r, wt_v(-50 + i * 2 * r, -50 + j * 2 * r), wt_v(0, 0), wt_v(0, -9.8));
            wt_sph_partical *sp = wt_create_sph_partical(p);

            //if(i==0 && j==0) {
            //    p->vel.x=10;p->vel.y=10;
            //}
            wt_sph_add_partical(w->fluid, sp);
        }
    }
}

void run()
{
    wt_world_run();
    wt_draw(w_world);
    //_sleep(10);
}

void runPhy()
{
    wt_world_int();
    w_world = wt_get_world();
    wt_generate_body(w_world);
    //wt_generate_fluid(w_world);
    wt_gl_main(&run);

}

int main()
{
    runPhy();

    //float a = 0;
    //float b = (a == WT_MAX_R32) ?  0 : ((a == 0) ? WT_MAX_R32 : 1 / a);

    //float c = WT_MAX_R32;

    //wt_debug("%f\n", b);
    //wt_debug("%f", c);
    void *obj = 0;
    wt_spatial_hash *h = wt_init_spatial_hash(1024, 5);

    wt_add_to_spatial_hash(h, wt_v(1, 1), wt_v(1, 1), obj);

    wt_debug("key %d \n",wt_get_spatial_hash_key(59,128,1024));

    wt_debug("19349663 * 128 %d \n", 19349663 * 128);


}

// int main ()
// {
//  //wt_body *b = wt_create_body0(100.0, wt_v(0,0), 90);
//  //wt_debug(" %f", b->fric);

//  wt_world_int();

//  wt_world* w = wt_get_world();

//  wt_body *b1 = wt_create_body0(10.0, wt_v(0,0), 10.0);
//  b1->vel.x = 10.0;

//  wt_circle *c1 = wt_create_cir(b1, 10.0);

//  wt_shape *s1 = wt_create_shape(c1, WT_CIR);

//  if(w==NULL){
//      wt_debug("w is NULL", 1);
//  }

//  wt_world_add_shape(w,s1);

//  wt_world_run();

// }
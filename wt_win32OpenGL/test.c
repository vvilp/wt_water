
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
            wt_r32 x = w->width / 2 + j * 2 * r ;
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

void wt_generate_fluid_partical(float x, float y)
{
    wt_partical *p = wt_create_partical(1, 20, wt_v(x, y), wt_v(0, 0), wt_v(0, 0));
    wt_pvf_partical *pvf_p = wt_create_pvf_partical(p);
    wt_pvf_add_partical(w_world->fluid, pvf_p);
}

void wt_generate_fluid(wt_world *w)
{
    wt_partical *p = wt_create_partical(1, 20, wt_v(50, 50), wt_v(0, 0), wt_v(0, 0));
    wt_pvf_partical *pvf_p = wt_create_pvf_partical(p);
    wt_pvf_add_partical(w->fluid, pvf_p);
}

void run()
{
    wt_world_run();
    wt_draw(w_world);
    _sleep(10);
}

void keyboard(unsigned char c, __attribute__((unused)) int x, __attribute__((unused))  int y)
{
    //float radius = SIM_W/8;
    // wt_debug("test keybord", 1);
    switch (c)
    {
        // Quit
    case 'q':
        break;
    case 'Q':
        exit(0);
        break;
    case 'a':
        wt_debug("test keybord", 1);
        break;

    }
}

int isMouseDown = 0;
void Mouse(int button, int state, int x, int y)
{
    float tmp_x = (float) x;
    float tmp_y = (float) y;
    tmp_x = x / 400.0 * w_world->width;
    tmp_y = y / 400.0 * w_world->width;
    //float real_x = 100 - 400 - tmp_x;
    float real_x =  tmp_x;
    float real_y = w_world->width - tmp_y;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        //wt_debug("%f,%f\n", real_x, real_y);
        wt_generate_fluid_partical(real_x,real_y);
        isMouseDown = 1;
    }

    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){
        isMouseDown = 0;
    }

}

void mouseMove(int x,int y)
{
    float tmp_x = (float) x;
    float tmp_y = (float) y;
    tmp_x = x / 400.0 * w_world->width;
    tmp_y = y / 400.0 * w_world->width;
    //float real_x = 100 - 400 - tmp_x;
    float real_x =  tmp_x;
    float real_y = w_world->width - tmp_y;
    if(isMouseDown){
         wt_generate_fluid_partical(real_x,real_y);
    }
   
}

void wt_gl_reshape1 ( int w, int h )   // Create The Reshape Function (the viewport)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(120.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
    gluOrtho2D(0, 100, 0, 100); //左下角x坐标，右上角x坐标，左下角y坐标，右上角y坐标
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, -15.0);
}


void runPhy()
{
    wt_world_int();
    w_world = wt_get_world();
    //wt_generate_body(w_world);
    wt_generate_fluid(w_world);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowPosition(350, 350);
    glutInitWindowSize(400, 400);
    glutCreateWindow("waterZ");
    wt_gl_init ();//因为里面的抗锯齿,需要在创建窗口后调用才行
    glutReshapeFunc(wt_gl_reshape1);
    glutDisplayFunc(run);
    glutIdleFunc(run);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(Mouse);
    glutMotionFunc(mouseMove);
    glutMainLoop();
}

int main()
{
    runPhy();

}

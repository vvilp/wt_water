
#include "../wt_core/wt_core.h"
#include "wt_gl.h";
#include "time.h"
wt_world *w_world = NULL;

wt_r32 window_w = 0;
wt_r32 window_h = 0;
wt_i32 mouse_x = 0;
wt_i32 mouse_y = 0;
int isLeftMouseDown = 0;
int isRightMouseDown = 0;
wt_array *choose_particals = NULL;




void wt_generate_body(wt_world *w)
{
    //wt_circle_pyramid(w);
    //wt_cir_wall_bottom(w);
}

void run()
{
    clock_t start, finish;
    start = clock();
    wt_world_run();
    wt_draw(w_world);
    finish = clock();
    wt_debug("FPS : %f\r", 1.0 / ((double)(finish - start) / CLOCKS_PER_SEC));
    //_sleep(5);

}

void wt_mouse_func()
{
    if (isLeftMouseDown)
    {
        wt_generate_fluid_partical(w_world, mouse_x, mouse_y, 5);
        //wt_generate_circle(mouse_x,mouse_y,0.5);
        if (w_world->fluid->pvf_particals->num != 0 && w_world->fluid->pvf_particals->num % 1000 == 0)
            wt_debug("partical Num: %d \n", w_world->fluid->pvf_particals->num);
    }

    if (isRightMouseDown)
    {
        wt_pvf_add_extern_force(choose_particals, 2, wt_v(mouse_x, mouse_y));
        //wt_debug("partical Num: %d \n", w_world->fluid->pvf_particals->num);
    }
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


void Mouse(int button, int state, int x, int y)
{
    float tmp_x = (float) x;
    float tmp_y = (float) y;
    tmp_x = x / window_w * w_world->width;
    tmp_y = y / window_h * w_world->width;
    //float real_x = 100 - 400 - tmp_x;
    float real_x =  tmp_x;
    float real_y = w_world->width - tmp_y;
    //wt_debug("asd \n", 1);
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        //wt_debug("%f,%f\n", real_x, real_y);
        wt_generate_fluid_partical(w_world,real_x, real_y, 5);
        //wt_generate_circle(real_x, real_y, 0.5);
        isLeftMouseDown = 1;
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        //wt_debug("%f,%f\n", real_x, real_y);
        //wt_generate_fluid_partical(real_x, real_y, 30);
        //isMouseDown = 1;
        if (choose_particals == NULL) choose_particals = wt_array_init(50);
        //wt_vec ael = wt_v(0,10.0);
        //wt_vec pos =
        wt_pvf_choose_range_particals(w_world->fluid->pvf_particals, wt_v(real_x, real_y), 12, choose_particals);
        isRightMouseDown = 1;
        wt_debug("choose : %d \n", choose_particals->num);
        wt_generate_circle(w_world, real_x, real_y, 5);


    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        isLeftMouseDown = 0;
    }

    if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
    {
        isRightMouseDown = 0;
        wt_pvf_set_partical_ael(choose_particals, wt_v(0, -10.0));
        wt_array_clear(choose_particals);
    }

}

void mouseMove(int x, int y)
{
    float tmp_x = (float) x;
    float tmp_y = (float) y;
    tmp_x = x / window_w * w_world->width;
    tmp_y = y / window_h * w_world->width;
    float real_x =  tmp_x;
    float real_y = w_world->width - tmp_y;
    mouse_x = real_x;
    mouse_y = real_y;
    wt_mouse_func();
}


void wt_gl_reshape1 (int w, int h)   // Create The Reshape Function (the viewport)
{
    window_h = h;
    window_w = w;
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
    wt_generate_body(w_world);
    window_h = (wt_r32)window_size;
    window_w = (wt_r32)window_size;

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowPosition(350, 350);
    glutInitWindowSize(window_size, window_size);
    glutCreateWindow("WATER-Z-WZT");
    wt_gl_init ();//因为里面的抗锯齿,需要在创建窗口后调用才行
    glutReshapeFunc(wt_gl_reshape1);
    glutDisplayFunc(run);
    glutIdleFunc(run);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(Mouse);
    glutMotionFunc(mouseMove);
    glutPassiveMotionFunc(mouseMove);
    glutMainLoop();
}

int main(int argc, char **argv)
{
    //linux need
    glutInit(&argc, argv);
    runPhy();
}

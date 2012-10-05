
#include "../wt_core/wt_core.h"
#include "wt_gl.h";
#include "time.h"
//wt_world *w_world = NULL;


#define WT_ADD_CIR 1
#define WT_ADD_FLUID 2

wt_r32 window_w = 0;
wt_r32 window_h = 0;
wt_i32 mouse_x = 0;
wt_i32 mouse_y = 0;
int isLeftMouseDown = 0;
int isRightMouseDown = 0;
int wt_add_type = 0;
wt_array *choose_particals = NULL;


void wt_generate_body(wt_world *w)
{
    //wt_circle_pyramid(w);
    //wt_cir_wall_bottom(w);
    //wt_circle_matrix(w);
    //wt_cir_wall_bottom(w);
}

void run()
{
    wt_world *w_world = wt_get_world();
    clock_t start, finish;
    start = clock();
    wt_world_run();
    wt_draw(w_world);
    finish = clock();
    wt_debug("FPS : %f\r", 1.0 / ((double)(finish - start) / CLOCKS_PER_SEC));
    //_sleep(5);
}

float add_cir_mass = WT_MAX_R32;
void wt_add_things(int x, int y)
{
    wt_world *w_world = wt_get_world();
    switch (wt_add_type)
    {
    case WT_ADD_CIR:
        wt_generate_circle(w_world, x, y, 4, add_cir_mass);
        break;
    case WT_ADD_FLUID:
        wt_generate_fluid_partical(w_world, x, y, 5);
        break;
    }
}

wt_vec wt_get_world_xy(int x, int y)
{
    wt_world *w_world = wt_get_world();
    float tmp_x = (float) x;
    float tmp_y = (float) y;
    tmp_x = x / window_w * w_world->width;
    tmp_y = y / window_h * w_world->width;
    float real_x = tmp_x;
    float real_y = w_world->width - tmp_y;
    return wt_v(real_x, real_y);
}

void keyboard(unsigned char c, int x, int y)
{
    wt_world *w_world = wt_get_world();
    wt_r32 tmp = 0;
    int mod = glutGetModifiers();
    switch (c)
    {
    case 'f':
        wt_add_type = WT_ADD_FLUID;
        wt_debug("Add fluid \n", 1);
        break;
    case 'c':
        wt_add_type = WT_ADD_CIR;
        wt_debug("Add circle \n", 1);
        break;
    case 'q':
        tmp = wt_world_get_pvf_plastic(w_world);
        tmp = mod == GLUT_ACTIVE_CTRL ? tmp - 50 : tmp + 50;
        wt_world_set_pvf_plastic(w_world, tmp);
        wt_debug("set fluid plastic : %f \n", tmp);
        break;
    case 'w':
        tmp = wt_world_get_pvf_plastic(w_world);
        wt_world_set_pvf_plastic(w_world, tmp - 50.0);
        wt_debug("set fluid plastic : %f \n", tmp - 100.0);
        break;
    case 'e':
        tmp = wt_world_get_pvf_viscosity(w_world);
        wt_world_set_pvf_viscosity(w_world, tmp + 1.0);
        wt_debug("set fluid viscosity : %f \n", tmp + 1.0);
        break;
    case 'r':
        tmp = wt_world_get_pvf_viscosity(w_world);
        wt_world_set_pvf_viscosity(w_world, tmp - 1.0);
        wt_debug("set fluid viscosity : %f \n", tmp - 1.0);
        break;
    case 't':
        tmp = wt_world_get_pvf_temperaturefactor(w_world);
        wt_world_set_pvf_temperaturefactor(w_world, tmp + 0.001);
        wt_debug("set fluid T : %f \n", tmp + 0.001);
        break;
    case 'y':
        tmp = wt_world_get_pvf_temperaturefactor(w_world);
        wt_world_set_pvf_temperaturefactor(w_world, tmp - 0.001);
        wt_debug("set fluid T : %f \n", tmp - 0.001);
        break;

    case 'u':
        tmp = wt_world_get_pvf_density(w_world);
        wt_world_set_pvf_density(w_world, tmp + 100);
        wt_debug("set fluid density : %f \n", tmp + 100);
        break;
    case 'i':
        tmp = wt_world_get_pvf_density(w_world);
        wt_world_set_pvf_density(w_world, tmp - 100);
        wt_debug("set fluid density : %f \n", tmp - 100);
        break;
    case '1':
        wt_world_clear_all(w_world);
        wt_circle_pyramid(w_world);
        wt_cir_wall_bottom(w_world);
        break;
    case '2':
        wt_world_clear_all(w_world);
        wt_circle_matrix(w_world);
        wt_cir_wall_bottom(w_world);
        break;
    case 'm':
        wt_world_clear_all(w_world);
        break;
    }
}


void Mouse(int button, int state, int x, int y)
{
    wt_world *w_world = wt_get_world();

    wt_vec pos = wt_get_world_xy(x, y);

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        isLeftMouseDown  = 1;
        wt_add_things(pos.x, pos.y);
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        isRightMouseDown = 1;
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        isLeftMouseDown  = 0;
    }

    if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
    {
        isRightMouseDown = 0;
    }

}

void mouseMove(int x, int y)
{
    wt_vec pos = wt_get_world_xy(x, y);

    if (isLeftMouseDown && wt_add_type == WT_ADD_FLUID)
    {
        wt_add_things(pos.x, pos.y);
    }
}


void wt_gl_reshape1 (int w, int h)
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
    wt_world *w_world = wt_get_world();
    wt_generate_body(w_world);
    window_h = (wt_r32)window_size;
    window_w = (wt_r32)window_size;

    //glutInitWindowPosition(350, 350);
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
    glutInit(&argc, argv);                           // GLUT Initializtion
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);     // Display Mode (Rgb And Double Buffered)
    runPhy();
}

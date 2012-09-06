
#include "wt_main.h"


void wt_generate_body(wt_world *w)
{
    //LOGI("add body");
    //wt_circle_pyramid(w);
    //wt_circle_matrix(w);
    //wt_cir_wall_bottom(w);
}

void runPhy()
{
    wt_world_run();
    //wt_draw();
    wt_world *w_world = wt_get_world();
    wt_draw(w_world);
}

void initPhy()
{
    wt_world_int();
    wt_world *world = wt_get_world();
    wt_generate_body(world);
}

//----------------------------------------------------

void InitGL() {  
    wt_gl_init();
    initPhy();
}  

void SizeChanged(int width ,int height) {  
    glViewport(0, 0, width, height);//修改定义显示视窗的大小和位置 从左下角开始(x,y,width,height)
    float ratio = width / (float) height;
    glMatrixMode(GL_PROJECTION); // set matrix to projection mode
    glLoadIdentity(); //将当前的用户坐标系的原点移到了屏幕中心：类似于一个复位操作
    glFrustumf(-ratio, ratio, -1, 1, -1, 7); //此方法，前面4个参数用于确定窗口的大小，而后面两个 参数分别是在场景中所能绘制深度的起点和终点。 
    glOrthof(0, 100, 0, 100,-1,1);
    glMatrixMode(GL_MODELVIEW);
}  

void RendererGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除屏幕和深度缓存
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    runPhy();
}

void wt_world_add_fluid(wt_r32 x , wt_r32 y)
{
    wt_world *w_world = wt_get_world();
    wt_generate_fluid_partical(w_world, x, y, 5);
}



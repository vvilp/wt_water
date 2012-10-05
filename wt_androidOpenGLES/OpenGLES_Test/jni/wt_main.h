#ifndef _WT_MAIN_H_
#define _WT_MAIN_H_

#include "wt_android_core.h"

//void draw();
//void runPhy();
//void initPhy();
//void wt_setGravity(float x,float y);
#define RIGID_DEMO1 1
#define RIGID_DEMO2 2
#define RIGID_DEMO3 3
#define SOFT_DEMO1 4
#define SOFT_DEMO2 5
#define SOFT_DEMO3 6

static int reset_demo_index = 0 ;
static int demo_index = 0;
void wt_reset_demo_index(int index);

int wt_get_demo_index();
void wt_reset_demo(wt_world *world);
void InitGL();
void SizeChanged(int width ,int height);
void RendererGL();
void wt_world_add_fluid(wt_r32 x , wt_r32 y);
#endif
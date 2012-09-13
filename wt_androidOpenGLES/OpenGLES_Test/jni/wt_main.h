#ifndef _WT_MAIN_H_
#define _WT_MAIN_H_

#include "wt_android_core.h"

//void draw();
//void runPhy();
//void initPhy();
//void wt_setGravity(float x,float y);

static int reset_demo_index = 0 ;
void wt_reset_demo_index(int index);

void wt_reset_demo(wt_world *world);
void InitGL();
void SizeChanged(int width ,int height);
void RendererGL();
void wt_world_add_fluid(wt_r32 x , wt_r32 y);
#endif
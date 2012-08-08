/*
Particle-based Viscoelastic Fluid Simulation
*/

#ifndef _WT_PVF_FUILD_H_
#define _WT_PVF_FUILD_H_
#include "wt_core.h"


typedef struct {
	wt_partical *partical;
	wt_r32 p_density;
	wt_r32 p_density_near;
	wt_r32 p_press;
	wt_r32 p_press_near;
} wt_pvf_partical;

typedef struct {
	wt_array* pvf_particals;
	wt_spatial_table *pvf_particals_table;
	wt_r32 density;//静态密度
	wt_r32 h;//粒子作用范围
	wt_r32 k;
	wt_r32 k_near;
	wt_r32 sigma;
	wt_r32 beta;
	wt_r32 partical_max_vel;
} wt_pvf_fluid;


wt_pvf_partical*	wt_create_pvf_partical(wt_partical * p);
wt_pvf_fluid*		wt_create_pvf_fluid();
void 				wt_pvf_add_partical(wt_pvf_fluid *f, wt_pvf_partical* p);
void 				wt_pvf_update_fluid(wt_pvf_fluid *f, wt_r32 dt);

#endif
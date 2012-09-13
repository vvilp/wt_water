#ifndef _WT_SPH_FLUID_H_
#define _WT_SPH_FLUID_H_

#include "wt_core.h"
//用于模拟流体粒子行为


typedef struct {
	wt_partical * partical;

	wt_r32 p_density;
	wt_vec ael_pressure;
	wt_vec ael_viscosity;
} wt_sph_partical;

typedef struct {
	wt_r32 density;//静态密度
	wt_r32 viscosity;//粘度
	wt_r32 h;//粒子作用范围
	wt_r32 k;//和流体相关的常数，只跟温度相关
	wt_array* sph_particals;//wt_sph_partical

	//核系数
	wt_r32 k_poly6;
	wt_r32 k_spiky;
	wt_r32 k_viscosity;

	//临时限制半径
	wt_r32 world_width;
} wt_sph_fluid;

wt_sph_partical* wt_create_sph_partical(wt_partical * partical);

wt_sph_fluid* wt_create_sph_fluid(wt_r32 density, wt_r32 viscosity, wt_r32 k, wt_r32 h);

void wt_sph_add_partical(wt_sph_fluid *fluid,wt_sph_partical *sp);

void wt_sph_update_density(wt_sph_fluid *fluid);

void wt_sph_update_ael_pressure(wt_sph_fluid *fluid);

void wt_sph_update_ael_viscosity(wt_sph_fluid *fluid);

void wt_sph_update_partical(wt_sph_fluid *fluid,wt_r32 dt);

void wt_sph_update_fluid(wt_sph_fluid *fluid, wt_r32 dt);


#endif
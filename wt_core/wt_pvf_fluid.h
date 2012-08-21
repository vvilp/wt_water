/*
Particle-based Viscoelastic Fluid Simulation
*/

#ifndef _WT_PVF_FUILD_H_
#define _WT_PVF_FUILD_H_
#include "wt_core.h"


typedef struct {
	//wt_partical *partical;
	wt_body *body;
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

	wt_r32 k_spring;//弹簧劲度系数
	wt_r32 spring_rest_len;
} wt_pvf_fluid;


//wt_pvf_partical*	wt_create_pvf_partical(wt_partical * p);
wt_pvf_partical*	wt_create_pvf_partical(wt_body * b);
wt_pvf_fluid*		wt_create_pvf_fluid();
void 				wt_pvf_add_partical(wt_pvf_fluid *f, wt_pvf_partical* p);
void 				wt_pvf_update_fluid(wt_pvf_fluid *f, wt_r32 dt);

void wt_pvf_choose_range_particals(wt_array *all_pvf_particals,wt_vec pos, wt_r32 range,wt_array *choose_particals);
void wt_pvf_add_extern_force(wt_array *pvf_particals,wt_r32 ael, wt_vec to_pos);
void wt_pvf_set_partical_ael(wt_array *pvf_particals, wt_vec ael);

// void wt_pvf_set_density					(wt_pvf_fluid *pvf, wt_r32 density);
// void wt_pvf_set_viscosity_sigma			(wt_pvf_fluid *pvf, wt_r32 sigma);
// void wt_pvf_set_viscosity_beta			(wt_pvf_fluid *pvf, wt_r32 beta);
// void wt_pvf_set_temperature_k			(wt_pvf_fluid *pvf, wt_r32 k);
// void wt_pvf_set_temperature_k_near		(wt_pvf_fluid *pvf, wt_r32 k_near);
// void wt_pvf_set_partical_action_range	(wt_pvf_fluid *pvf, wt_r32 range);
// void wt_pvf_set_partical_spring_k		(wt_pvf_fluid *pvf, wt_r32 k);
// void wt_pvf_set_partical_spring_range	(wt_pvf_fluid *pvf, wt_r32 range);
#endif
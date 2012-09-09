#ifndef _WT_WORLD_H_
#define _WT_WORLD_H_
#include "wt_core.h"


static const wt_i32 window_size = 500;
static const wt_i32 world_size = 100;
typedef struct
{
	wt_array* shapes;
	wt_array* contacts;

	wt_vec gravity;

	wt_pvf_fluid *fluid;
	wt_vec fluid_gravity;

	//空间哈希方法，效率和简单循环竟然差不多。。。。
	//wt_spatial_hash *hash;

	//空间二维表方法, 效率比较高
	wt_spatial_table *shapes_table;

	wt_r32 width;
} wt_world;


wt_world* 		wt_create_world();
void 			wt_world_int();
void 			wt_world_run();
wt_world* 		wt_get_world();
wt_status 		wt_world_add_shape(wt_world* w, wt_shape* s);
void 			wt_world_step(wt_r32 dt);
wt_status 		wt_world_update_bodys(wt_world *w, float wt_time);

void wt_world_clear_shapes(wt_world * world);
void wt_world_clear_all(wt_world * world);

void 	wt_world_set_gravity(wt_world* w,wt_vec gravity);
void 	wt_world_set_fluid_gravity(wt_vec gravity);
wt_vec  wt_world_get_fluid_gravity();
void 	wt_world_set_pvf_plastic(wt_world *w, wt_r32 plastic);
wt_r32  wt_world_get_pvf_plastic(wt_world *w);
void 	wt_world_set_pvf_viscosity(wt_world *w, wt_r32 viscosity);
wt_r32 	wt_world_get_pvf_viscosity(wt_world *w);
void 	wt_world_set_pvf_temperaturefactor(wt_world *w, wt_r32 temperature);
wt_r32  wt_world_get_pvf_temperaturefactor(wt_world *w);
void 	wt_world_set_pvf_density(wt_world *w, wt_r32 density);
wt_r32 	wt_world_get_pvf_density(wt_world *w);
#endif
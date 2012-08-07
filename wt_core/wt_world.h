#ifndef _WT_WORLD_H_
#define _WT_WORLD_H_
#include "wt_core.h"
typedef struct
{
	wt_array* shapes;
	wt_array* contacts;

	wt_r32 ALLOW_PENETRATION;//碰撞时允许贯穿量
	wt_vec gravity;

	//wt_sph_fluid *fluid;
	wt_pvf_fluid *fluid;
	wt_spatial_hash *hash;

	wt_r32 width;
} wt_world;


wt_world* 		wt_create_world();
void 			wt_world_int();
void 			wt_world_run();
wt_world* 		wt_get_world();
wt_status 		wt_world_add_shape(wt_world* w, wt_shape* s);
void 			wt_world_step(wt_r32 dt);
wt_status 		wt_world_update_bodys(wt_world *w, float wt_time);
void 			wt_world_set_gravity(wt_world* w,wt_vec gravity);

#endif
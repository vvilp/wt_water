#include "wt_world.h"


wt_world* wt_create_world()
{
	wt_world* w = NULL;
	w = (wt_world*) malloc (sizeof(wt_world)); 
	w -> shapes   = wt_array_init(100);
	w -> contact  = wt_array_init(100);
	w -> liquid   = wt_array_init(5);
	w -> springs  = wt_array_init(5);
	return w;
}

wt_status wt_world_add_shape(wt_world* w, wt_shape* s)
{
	if(wt_array_add(w->shapes, s) == WT_OK)
	 	return WT_OK;
	else
		return WT_ER;
} 

wt_status wt_world_add_spring(wt_world* w, wt_spring* s)
{
	wt_array_add(w->springs, s);
}

wt_status wt_energy(wt_world* w){
	wt_array* ss = w -> shapes;
	wt_r32 e = 0;
	for (int i = 0; i < ss -> num; ++i)
	{
		wt_shape* s = ss -> array[i];
		wt_body*  b = wt_shape_get_body(s);
		e += b->mas * wt_vlen(b->vel) /2;
		//printf("质量: %f\n",b->mas);
		//printf("速度: %f\n",wt_vlen(b->vel));
	}
	printf("动能总和: %f\n",e);
}

wt_status wt_world_update_step(wt_world* w, float wt_time)
{
	if(w == NULL) return WT_ER;
	//更新弹簧受力
	wt_world_update_springs(w->springs);
	//更新物体
	wt_world_update_bodys(w, wt_time);
	//wt_energy(w);
	//更新liquid
	wt_world_update_liquid(w, wt_time);
	return WT_OK;
}

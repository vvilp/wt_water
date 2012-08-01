#include "wt_body.h"

wt_body* wt_create_body(wt_r32 mas,wt_vec pos,wt_vec ael,wt_vec vel,wt_r32 ang_ael,wt_r32 ang_vel,wt_r32 angular,wt_r32 fric )
{
	wt_body* b = NULL;
	b = (wt_body*)malloc(sizeof(wt_body)); 
	b -> mas = mas;
	if(mas == WT_MAX_R32){
		b->inv_mas = 0;
	}else if(mas == 0){
		b->inv_mas = WT_MAX_R32;
	}else{
		b->inv_mas = 1 / mas;
	}
	b -> pos.x = pos.x; b -> pos.y = pos.y;
	b -> vel.x = vel.x; b -> vel.y = vel.y;
	b -> ael.x = ael.x; b -> ael.y = ael.y;
	b -> ang_ael = ang_ael; b -> ang_vel = ang_vel; b -> angular = angular;
	b -> fric = fric;
	b -> I = 0;
	return b;
}

wt_body* wt_create_body0(wt_r32 mas,wt_vec pos,wt_r32 angular) {
	return wt_create_body(mas,pos,wt_v(0,0),wt_v(0,0),0,0,angular,0);
}

wt_status wt_body_update_pos(wt_body *b, wt_vector v)
{
	b->pos = v;
	return WT_OK;
}

wt_status wt_body_update_vel(wt_body *b, wt_vector v)
{
	b->vel = v;
	return WT_OK;
}

wt_status wt_body_update_ael(wt_body *b, wt_vector v)
{
	b->ael = v;
	return WT_OK;
}

wt_status wt_body_update_per_step(wt_body *b)
{
	if(b == NULL) return WT_ER;
	wt_vector pos,vel,ael;
	pos = b -> pos; vel = b -> vel; ael = b -> ael;
	pos.x += vel.x; pos.y += vel.y; 
	vel.x += ael.x; vel.y += ael.y;
	b -> pos = pos; b -> vel = vel; b->ael = ael;
	return WT_OK;
}

//当角度大于2*pi 或 小于 -2*pi 重置角度
wt_r32 wt_reset_angular(wt_r32 a){
	if(wt_cmp_real(a,WT_PI*2) == 1)     return a - WT_PI*2;
    if(wt_cmp_real(a,-WT_PI*2) == -1)  	return a + WT_PI*2;
    return a;
}
wt_status wt_body_update_step(wt_body *b, float wt_dtime)
{
	if(b == NULL) return WT_ER;
	wt_vector pos,vel,ael;

	pos = b -> pos; vel = b -> vel;
	ael = wt_vdiv(b->force,b->mas);
	if(b->mas == WT_MAX_R32){
		ael = wt_v(0,0);
	}
	pos.x += vel.x * wt_dtime; pos.y += vel.y * wt_dtime;
	vel.x += ael.x * wt_dtime; vel.y += ael.y * wt_dtime;


	wt_r32 a_ael = b -> ang_ael;
	wt_r32 a_vel = b -> ang_vel;
	wt_r32 ang   = b -> angular;

    ang = ang + a_vel*wt_dtime;
    ang = wt_reset_angular(ang);
    a_vel += a_ael * wt_dtime;

    b -> ang_ael = a_ael; b -> ang_vel = a_vel; b -> angular = ang;
	b -> pos = pos; b -> vel = vel; b->ael = ael;

	return WT_OK;
}



//=======================================

#if 0
int main()
{


	//printf("%5.2f\n",b->pos.x);
}
#endif

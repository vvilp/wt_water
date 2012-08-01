#ifndef WT_BODY_H
#define WT_BODY_H
#include "wt_core.h"

typedef struct {
	wt_r32     mas,inv_mas;
	wt_vector  pos;//重心位置
	wt_vector  vel;//速度
	wt_vector  ael;//加速度
	wt_vector  force;//受力
    //wt_r32     vel_damp; //单位时间内每次运行后速度的损失
    //二维情况下,角加速度 角速度都可以使用标量,正负表示方向
    wt_r32 ang_ael;//二维角加速度;
    wt_r32 ang_vel;//二维角速度
    wt_r32 angular;//角度 弧度
    //表面摩擦系数；
    wt_r32 fric;
    //转动惯量
    wt_r32 I;
    //冲量
    //wt_vec impluse;
    wt_r32 restitution;

} wt_body;

wt_body* wt_create_body(wt_r32 mas,wt_vec pos,wt_vec ael,wt_vec vel,wt_r32 ang_ael,wt_r32 ang_vel,wt_r32 angular,wt_r32 fric,wt_vec force);
wt_body* wt_create_body0(wt_r32 mas,wt_vec pos,wt_r32 angular);
wt_status wt_update_body_pos(wt_body *b, wt_vector v);
wt_status wt_update_body_vel(wt_body *b, wt_vector v);
wt_status wt_update_body_ael(wt_body *b, wt_vector v);
wt_status wt_body_update_per_step(wt_body *b);
wt_status wt_body_update_step(wt_body *b, wt_r32 wt_time);

//通过角速度，求某向量的线速度   向量以起始点转动
static wt_vec wt_linear_vel(wt_r32 w, wt_vec p_c)
{
	wt_vec l_v;
	l_v.x = -w * p_c.y;
	l_v.y = w * p_c.x;
	return l_v;
}
//通过线速度，求某向量的角速度   向量以起始点转动 返回弧度
//a b叉积   正：a在b顺时针 负：a在b逆时针
static wt_r32 wt_angle_vel(wt_vec v, wt_vec p)
{
	return wt_vmulv(v, p) > 0 ? -(wt_vlen(v) / wt_vlen(p)) : (wt_vlen(v) / wt_vlen(p));
}

//动量
static wt_vec wt_vmntm(wt_body *b)
{
	wt_vec mntm;
	mntm.x = b->vel.x * b->mas;
	mntm.y = b->vel.y * b->mas;
	return mntm;
}

//角动量
static wt_r32 wt_wmntm(wt_body *b)
{
	return (b->ang_vel) * b->I;
}

static wt_r32 wt_body_energy(wt_body *b)
{
	return b->mas == WT_MAX_R32 ? 0 : wt_vlen2(b->vel) * b->mas / 2 + b->I * b->ang_vel * b->ang_vel / 2;
}
#endif

#ifndef _WT_STRUCT_H
#define _WT_STRUCT_H
#include "wt_vector.h"
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

} wt_body;
typedef struct {
	wt_array* shapes;
	wt_array* springs;
	wt_array* contact;
	wt_array* liquid;
} wt_world;
typedef struct {
	wt_body *b1;
	wt_body *b2;
	wt_r32 relaxlen;//松弛长度
	wt_r32 maxlen;  //最大形变长度
	wt_r32 coef;    //弹性系数
	wt_r32 damp;
} wt_spring;
typedef struct {
	wt_array *bodys;
	wt_i32 force_num;
	wt_vec pos;//位置
	wt_array *springs;
} wt_liquid;

#endif

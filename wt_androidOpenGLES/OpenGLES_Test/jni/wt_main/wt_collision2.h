#ifndef WT_COLLIDE_H
#define WT_COLLIDE_H
#include "wt_body.h"
#include "wt_world.h"
#include "wt_vector.h"
#include "wt_shape_collision.h"
#include <stdio.h>


/**
 * 求碰撞点的冲量
 * 3 碰撞点的相对速度
 * 4 碰撞点
 * 5 作用线单位向量
 * 6 系数 1完全弹性碰撞
 */
static wt_r32 wt_impulse(wt_body* b1, wt_body* b2, wt_vec v_b1b2, wt_vec c_p, wt_vec apply_uvec, wt_r32 e)
{
	wt_r32 t1 = -(1 + e);
	wt_r32 t2 = wt_vdot(v_b1b2, apply_uvec);
	wt_r32 t3 = 1 / b1->mas;
	wt_r32 t4 = 1 / b2->mas;
	wt_r32 t5 = wt_vmulv(wt_vsub(c_p, b1->pos), apply_uvec);
	t5 = t5 * t5 / b1->I;
	wt_r32 t6 = wt_vmulv(wt_vsub(c_p, b2->pos), apply_uvec);
	t6 = t6 * t6 / b2->I;

	printf("t1  : %f \n" , t1);
	printf("t2  : %f \n" , t2);
	debug_vec(v_b1b2,"v_b1b2");
	debug_vec(apply_uvec,"apply_uvec");
	printf("t3 + t4 + t5 + t6 : %f \n" , t3 + t4 + t5 + t6);
	return t1 * t2 / (t3 + t4 + t5 + t6);
}

/**
 * 通过冲量更新物体
 * 暂时不考虑摩擦
 */
static wt_status wt_impulse_update_body(wt_body* b1, wt_body* b2, wt_r32 impulse, wt_vec c_p, wt_vec apply_uvec)
{
	wt_vec dv1 = wt_vmuls(apply_uvec, impulse / b1->mas);
	wt_vec dv2 = wt_vmuls(apply_uvec, impulse / b2->mas);
	wt_r32 dw1 = wt_vmulv(wt_vsub(c_p, b1->pos), wt_vmuls(apply_uvec, impulse)) / b1->I;
	wt_r32 dw2 = wt_vmulv(wt_vsub(c_p, b2->pos), wt_vmuls(apply_uvec, impulse)) / b2->I;
	b1->vel = wt_vadd(b1->vel, dv1);
	b2->vel = wt_vsub(b2->vel, dv2);
	b1->ang_vel = b1->ang_vel + dw1;
	b2->ang_vel = b2->ang_vel - dw2;
}

/**
 *获取碰撞点的线速度
 */
static wt_vec wt_collision_point_vel(wt_body* b, wt_vec c_p)
{
	//碰撞点 到 重心的向量
	wt_vec p_c = wt_vsub(c_p, b->pos);
	//碰撞点的旋转线速度
	wt_vec v_linear = wt_linear_vel(b->ang_vel, p_c);
	//碰撞点的速度
	return wt_vadd(b->vel, v_linear);
}

static wt_r32 wt_contact_impulse(wt_contact *c){
	wt_body *b1 = c->b1;
	wt_body *b2 = c->b2;
	wt_vec v1_b1_p = wt_collision_point_vel(b1, c->contact_p);//碰撞点的初始速度
	wt_vec v1_b2_p = wt_collision_point_vel(b2, c->contact_p);
	wt_vec v1_b1b2 = wt_vsub(v1_b1_p, v1_b2_p);//b1 b2 相对速度
	//计算冲量
	wt_r32 impulse = wt_impulse(b1, b2, v1_b1b2, c->contact_p, c->aply_uv, 1.0f);

	return impulse;
}

/**
 * 检测有所有碰撞
 */
static wt_status wt_scanCollision(wt_world *w)
{
	if (w == NULL)
		return WT_ER;
	wt_array *ss = w->shapes;
	for (int i = 0; i < ss->num; ++i)
	{
		wt_shape *s1 = ss->array[i];
		for (int j = i + 1; j < ss->num; ++j)
		{
			wt_shape *s2 = ss->array[j];
			if(wt_shape2shape(*s1, *s2, w->contact)){
				//_sleep(1000);
			}
		}
	}
	return WT_OK;
}

/**
 * 计算每个物体的总冲量
 */
static wt_status wt_update_body_impulse(wt_world *w){

	wt_array *contacts = w->contact;
	for(int i = 0 ; i < contacts->num ; i++){
		wt_contact *c = contacts->array[i];
		//计算每个碰撞点的冲量
		wt_r32 impls = wt_contact_impulse(c);
		printf("impluse:%f\n",impls);
		//更新到物体的冲量(向量)
		wt_impulse_update_body(c->b1,c->b2,impls,c->contact_p,c->aply_uv);
	}
}

///**
// * 通过冲量更新物体
// */
static wt_status wt_update_body_by_impulse(wt_world *w){



}

/**
 * 检测碰撞 更新body
 */
static wt_status wt_collision_update(wt_world *w){
	//printf("碰撞检测\n");
	//检测所有碰撞
	wt_scanCollision(w);
	//计算每个物体的总冲量(向量)
	wt_update_body_impulse(w);

	//wt_update_body_by_impulse(w);

	//wt_clear_array_contact(w->contact);
}

//=========================================================================================




#endif

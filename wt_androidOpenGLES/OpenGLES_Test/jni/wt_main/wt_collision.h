#ifndef WT_COLLIDE_H
#define WT_COLLIDE_H
#include "wt_body.h"
#include "wt_world.h"
#include "wt_vector.h"
#include "wt_shape_collision.h"
#include <stdio.h>



static wt_collision* wt_create_collision(wt_shape *a,wt_shape *b,wt_r32 e,wt_r32 u){
	wt_collision* c = (wt_collision*) malloc (sizeof(wt_collision));
	c->a = a;
	c->b = b;
	c->e =e;
	c->u =u;
	//c->contacts = wt_array_init(2);
	return c;
}

static wt_status wt_collision_add_contact(wt_collision *cls,wt_contact *c){
	wt_array_add(cls->contacts, c);
}
static wt_status wt_free_collision(wt_collision **cls){
	wt_array *contacts = (*cls)->contacts;
	for(int i=0;i<contacts->num;i++){
		wt_contact *c = contacts->array[i];
		free(c);
	}
	wt_array_free(&((*cls)->contacts));
	free(*cls);
}
static wt_status wt_free_collision_array(wt_array *cls_array){
	for(int i=0;i<cls_array->num;i++){
		wt_collision* c = cls_array->array[i];
		wt_free_collision(&c);
	}
	wt_array_free(&cls_array);
}
/**
 * 经过测试ok
 */
static wt_status wt_clear_collision_array(wt_array *cls_array){
	for(int i=0;i<cls_array->num;i++){
		wt_collision* c = cls_array->array[i];
		wt_free_collision(&c);
	}
	cls_array->num=0;
}
//=======================================================================
//计算点碰撞速度,基于能量守恒,动量守恒
static void wt_collide_vel(wt_vec v1, wt_r32 m1, wt_vec v2, wt_r32 m2, wt_r32 resCoef, wt_vec *res_v1, wt_vec *res_v2)
{
	wt_r32 t_mas = m1 + m2;
	wt_vec tmp_v1 = wt_vmuls(v1, m1 - m2 * resCoef);
	wt_vec tmp_v2 = wt_vmuls(v2, m2 + m2 * resCoef);
	*res_v1 = wt_vdiv(wt_vadd(tmp_v1, tmp_v2), t_mas);
	tmp_v2 = wt_vmuls(v2, m2 - m1 * resCoef);
	tmp_v1 = wt_vmuls(v1, m1 + m1 * resCoef);
	*res_v2 = wt_vdiv(wt_vadd(tmp_v1, tmp_v2), t_mas);
}

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
	return wt_vlen(b->vel) * b->mas / 2 + b->I * b->ang_vel * b->ang_vel / 2;
}

/**
 * 获取碰撞作用单位向量
 * 指向s1
 */
//static wt_vec wt_collision_apply_uvec(wt_shape* s1, wt_shape* s2, wt_vec c_p)
//{
//	wt_vec apply_vec;
//	wt_circle *c;
//	if (WT_CIR == s1->type)
//	{
//		c = (wt_circle *) s1->shape;
//		apply_vec = wt_vsub(c->body->pos, c_p);
//		//printf("s1 is cir \n");
//	}
//	else if (WT_CIR == s2->type)
//	{
//		c = (wt_circle *) s2->shape;
//		apply_vec = wt_vsub(c_p, c->body->pos);
//		//printf("s2 is cir \n");
//	}
//	return wt_vunit(apply_vec);
//}

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

/**
 * 求碰撞点的冲量
 * 3 碰撞点的相对速度
 * 4 碰撞点
 * 5 作用线单位向量
 * 6 系数 1完全弹性碰撞
 */
static wt_r32 wt_collision_impulse(wt_body* b1, wt_body* b2, wt_vec v_b1b2, wt_vec c_p, wt_vec apply_uvec, wt_r32 e)
{
	wt_r32 t1 = -(1 + e);
	wt_r32 t2 = wt_vdot(v_b1b2, apply_uvec);
	wt_r32 t3 = b1->mas == WT_MAX_R32 ? 0 : 1 / b1->mas;
	wt_r32 t4 = b2->mas == WT_MAX_R32 ? 0 : 1 / b2->mas;
	wt_r32 t5 = wt_vmulv(wt_vsub(c_p, b1->pos), apply_uvec);
	t5 = t5 * t5 / b1->I;
	wt_r32 t6 = wt_vmulv(wt_vsub(c_p, b2->pos), apply_uvec);
	t6 = t6 * t6 / b2->I;

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

	if(b1->mas == WT_MAX_R32){
		dv1 = wt_v(0,0);
		dw1 = 0;
	}

	if(b2->mas == WT_MAX_R32){
		dv2 = wt_v(0,0);
		dw2 = 0;
	}

//	debug_vec(dv1, "dv1");
//	debug_vec(dv2, "dv2");
//	debug_vec(apply_uvec, "apply_uvec");
//	printf("impulse/b1->mas: %f \n", impulse / b1->mas);
//	debug_vec(b1->vel, "before b1->vel");
//	debug_vec(b2->vel, "before b2->vel");
	b1->vel = wt_vadd(b1->vel, dv1);
	b2->vel = wt_vsub(b2->vel, dv2);
	b1->ang_vel = b1->ang_vel + dw1;
	b2->ang_vel = b2->ang_vel - dw2;
//	debug_vec(b1->vel, "after b1->vel");
//	debug_vec(b2->vel, "after b2->vel");
}

/**
 * 碰撞更新
 * 1 body1
 * 2 body2
 * 3 碰撞点
 * 4 碰撞作用单位向量,由b2 指向 b1
 */
static wt_status wt_collision_update(wt_body* b1, wt_body* b2, wt_vec c_p, wt_vec apply_uvec)
{
	//求b1 - b2 相对速度
	wt_vec v1_b1_p = wt_collision_point_vel(b1, c_p);//碰撞点的初始速度
	wt_vec v1_b2_p = wt_collision_point_vel(b2, c_p);
	wt_vec v1_b1b2 = wt_vsub(v1_b1_p, v1_b2_p);//b1 b2 相对速度
	//计算冲量
	wt_r32 impulse = wt_collision_impulse(b1, b2, v1_b1b2, c_p, apply_uvec, 1.0f);
	printf("impluse:%f\n",impulse);
	//通过冲量更新物体
	wt_impulse_update_body(b1, b2, impulse, c_p, apply_uvec);
}

/**
 * 碰撞更新shape
 */
static wt_status wt_shape_update(wt_shape* s1, wt_shape* s2, wt_array *array_c)
{
	wt_body *b1, *b2;
	b1 = wt_shape_get_body(s1);
	b2 = wt_shape_get_body(s2);
	printf("before collision energy : %f \n", wt_body_energy(b1)+wt_body_energy(b2));

	printf("contact num:%d\n",array_c->num);
	for(int i=0;i<array_c->num;i++){
		wt_contact *contact = array_c->array[i];
		//获取碰撞作用单位向量 b2 指向 b1
		//wt_vec apply_uvec = wt_collision_apply_uvec(s1, s2, contact->contact_p);
		//碰撞更新物体
		debug_vec(contact->aply_uv,"碰撞作用线");
		wt_collision_update(b1, b2,contact->contact_p, contact->aply_uv);
		//wt_collision_update(b1, b2,contact->contact_p, apply_uvec);
	}


	//计算物体能量
	printf("after collision energy : %f \n", wt_body_energy(b1)+wt_body_energy(b2));
}

/**
 * 系统整体能量计算 验证能量守恒
 */

static wt_r32 wt_system_energy(wt_world *w){
	wt_r32 total_e = 0;
	wt_array *ss = w->shapes;
	for (int i = 0; i < ss->num; ++i)
	{
		wt_shape *s1 = ss->array[i];
		wt_body *b = wt_shape_get_body(s1);
		total_e += wt_body_energy(b);
	}
	return total_e;
}

//简单检测碰撞
static wt_status scanCollide(wt_world *w)
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

			wt_array *array_c = wt_array_init(2);
			if (wt_shape2shape(*s1, *s2, array_c))
			{
				wt_collision* c = wt_create_collision(s1,s2,1,0.5);
				c->contacts = array_c;
				wt_array_add(w->collision,c);
				wt_shape_update(s1,s2,array_c);
			}else{
				wt_array_free(&array_c);
			}
		}
	}
	//wt_clear_collision_array(w->collision);
	return WT_OK;
}

static wt_status updateCollide(wt_world *w)
{
	//printf("before system energy : %f \n",wt_system_energy(w));
	scanCollide(w);
	//printf("after system energy : %f \n",wt_system_energy(w));
}

#if 0
//----------------------------------------------------------
//之前临时写的函数，暂时不用
//----------------------------------------------------------

// 物体1 2相撞 ，对于物体1的冲量
// resCoef：碰撞系数  apply_v：作用线单位向量，作用线垂直于接触面
// r1 r2:重心到碰撞点向量
static wt_r32 wt_impulse(wt_vec r1, wt_vec v1, wt_r32 m1,wt_r32 I1,
		wt_vec r2, wt_vec v2, wt_r32 m2,wt_r32 I2,
		wt_r32 resCoef,wt_vec apply_uv)
{
	wt_vec dv = wt_vsub(v1,v2);
	wt_r32 impluse = wt_vdot(dv,apply_uv);
	wt_r32 ti1 = wt_vmulv(apply_uv,r1)/(I1);
	ti1 = wt_vmulv(wt_vmuls(apply_uv,ti1),r1);
	wt_r32 ti2 = wt_vmulv(apply_uv,r2)/(I2);
	ti2 = wt_vmulv(wt_vmuls(apply_uv,ti2),r2);
	impluse = (impluse) * (resCoef+1) / (1/m1 + 1/m2 + ti1 + ti2);

	printf("ti1:%f ti2:%f\n",ti1, ti2);
	return wt_fabs(impluse);
}

//通过作用点的冲量 更新物体速度 角速度
//apl_uv: 作用线单位向量 指向该物体
static void wt_update_body_by_impulse(wt_body *b,wt_vec cls_p,wt_vec apl_uv,wt_vec t_apl_uv,wt_r32 impulse)
{
	//碰撞点到重心向量
	wt_vec v_pos = wt_vsub( b->pos,cls_p);
	//v_pos = wt_unitv(v_pos);
	//垂直于作用线的向量，平行于接触面
	printf("impulse: %f\n",impulse);
	debug_vec(v_pos,"v_pos");
	debug_vec(t_apl_uv,"t_apl_uv");
	debug_vec(b->vel,"before b->vel");
	printf("before b->ang_vel : %f\n",b->ang_vel);

	wt_vec dv1, dv2;
	dv1 = wt_vmuls(apl_uv, impulse);
	dv2 = wt_vmuls(t_apl_uv, impulse * b->u);
	//dv  = wt_vadd(dv1, dv2) ;
	//debug_vec(dv,"dv");
	b->vel = wt_vadd(b->vel, wt_vdiv(dv1, b->mas));

	wt_r32 dw = wt_vmulv(dv1,v_pos) / (b->I);
	b->ang_vel = b->ang_vel + (dw);
	printf("dw : %f\n",dw);

	debug_vec(b->vel,"after b->vel");
	printf("after b->ang_vel : %f\n",b->ang_vel);
}

static void wt_update_vel2(wt_body *b1, wt_body *b2, wt_vec collide_pos, wt_vec apply_v)
{
	wt_vec v1 = b1->vel;wt_r32 w1 = b1->ang_vel * WT_PI / 180;wt_r32 m1 = b1->mas;
	wt_vec v2 = b2->vel;wt_r32 w2 = b2->ang_vel * WT_PI / 180;wt_r32 m2 = b2->mas;

	//计算过重心的速度和切向速度
	//碰撞点到重心向量
	wt_vec v_pos1 = wt_vsub(collide_pos, b1->pos);wt_vec v_pos2 = wt_vsub(collide_pos, b2->pos);

	//计算碰撞点线速度
	wt_vec l_v1 = wt_linear_vel(w1,v_pos1);wt_vec l_v2 = wt_linear_vel(w2,v_pos2);

	//分别计算碰撞点的整体速度
	wt_vec total_v1 = wt_vadd(v1, l_v1);wt_vec total_v2 = wt_vadd(v2, l_v2);

	wt_r32 impulse = wt_impulse(v_pos1,total_v1, m1, b1->I , v_pos2, total_v2, m2,b2->I, 1, apply_v);

	//相对速度
	wt_vec dv = wt_vsub(total_v1,total_v2);
	//垂直于作用线的向量 与相对速度v有关， (n X v) Xn
	wt_vec t_v;
	t_v.x = -wt_vmulv(apply_v,dv) * apply_v.y;t_v.y = wt_vmulv(apply_v,dv) * apply_v.x;
	t_v = wt_vunitv(t_v);
	t_v.x = -t_v.x; t_v.y = -t_v.y;
	//debug_vec(dv,"dv");

	debug_vec(wt_vmntm(b1),"b1 动量");
	debug_vec(wt_vmntm(b2),"b2 动量");
	printf("b1 b2角动量: %f %f\n",wt_wmntm(b1),wt_wmntm(b2));;

	wt_update_body_by_impulse(b1,collide_pos,apply_v,t_v, impulse);
	t_v.x = -t_v.x; t_v.y = -t_v.y;
	apply_v.x = -apply_v.x; apply_v.y=-apply_v.y;
	wt_update_body_by_impulse(b2,collide_pos,apply_v,t_v, impulse);

	debug_vec(wt_vmntm(b1),"b1 动量");
	debug_vec(wt_vmntm(b2),"b2 动量");

	printf("b1 b2角动量: %f %f\n",wt_wmntm(b1),wt_wmntm(b2));;

}
//计算 碰撞后速度 看来不能用该方法
//参数 物体1 - 物体2 移动速度 角速度 质量
//碰撞位置 作用线向量垂直于接触面
static void wt_update_vel(wt_body *b1, wt_body *b2, wt_vec collide_pos, wt_vec apply_v)
{
	wt_vec v1 = b1->vel;wt_r32 w1 = wt_atow(b1->angular);wt_r32 m1 = b1->mas;
	wt_vec v2 = b2->vel;wt_r32 w2 = wt_atow(b2->angular);wt_r32 m2 = b2->mas;

	//计算过重心的速度和切向速度
	//碰撞点到重心向量
	wt_vec v_pos1, v_pos2;
	v_pos1 = wt_vsub(collide_pos, b1->pos);v_pos2 = wt_vsub(collide_pos, b2->pos);
	//碰撞点垂直于中心的切向量   x!=0时为  -y/x,1 x==0时为 1,0
	wt_vec t_pos1, t_pos2;
	t_pos1 = wt_vertc_unitv(v_pos1);
	t_pos2 = wt_vertc_unitv(v_pos2);
	//t_pos1.x = v_pos1.x == 0 ? 1 : (-v_pos1.y / v_pos1.x);t_pos1.y = v_pos1.x == 0 ? 0 : 1;
	//t_pos2.x = v_pos2.x == 0 ? 1 : (-v_pos2.y / v_pos2.x);t_pos2.y = v_pos2.x == 0 ? 0 : 1;

	//计算碰撞点线速度
	wt_vec l_v1 = wt_linear_vel(w1,v_pos1);
	wt_vec l_v2 = wt_linear_vel(w2,v_pos2);

	//分别计算碰撞点的整体速度
	wt_vec total_v1 = wt_vadd(v1, l_v1);wt_vec total_v2 = wt_vadd(v2, l_v2);

	wt_vec after_v1, after_v2;
	//计算碰撞速度,碰撞损失系数,暂时定为0 弹性碰撞
	wt_collide_vel(total_v1, m1, total_v2, m2, 1, &after_v1, &after_v2);

	//after_v1 = wt_vproj(after_v1, apply_v);
	//after_v2 = wt_vproj(after_v2, apply_v);

	b1->vel = wt_vproj(after_v1, v_pos1);
	b2->vel = wt_vproj(after_v2, v_pos2);

	printf("after_v1 b1 : %f , %f \n",after_v1.x,after_v1.y);
	printf("after_v2 b2 : %f , %f \n",after_v2.x,after_v2.y);
	printf("b1 : %f , %f \n",b1->vel.x,b1->vel.y);
	printf("b2 : %f , %f \n",b2->vel.x,b2->vel.y);
	////	//切向速度
	wt_vec b1_tvel = wt_vproj(after_v1, t_pos1);
	wt_vec b2_tvel = wt_vproj(after_v2, t_pos2);
	//
	b1->ang_vel = wt_angle_vel(b1_tvel,v_pos1) * 180 / WT_PI;
	b2->ang_vel = wt_angle_vel(b2_tvel,v_pos2) * 180 / WT_PI;
	printf("b1_tvel : %f , %f \n",b1_tvel.x,b1_tvel.y);
	printf("b2_tvel : %f , %f \n",b2_tvel.x,b2_tvel.y);
	printf("ang_vel : %f , %f \n",b1->ang_vel,b2->ang_vel);

}
#endif
#endif

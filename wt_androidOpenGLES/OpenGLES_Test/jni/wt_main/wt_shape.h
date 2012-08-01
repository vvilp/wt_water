#ifndef WT_SHAPE_H
#define WT_SHAPE_H

#include "wt_body.h"

#define WT_CIR 1
#define WT_REC 2
typedef struct {
	wt_body *body;
	wt_r32 radius;
	//圆形转动惯量  MR^2/2
	//wt_r32 I;
} wt_circle;

typedef struct {
	wt_body *body;
	wt_r32 u_len; //中点距离上的距离
	wt_r32 r_len; //中点距离右的距离
	//矩形转动惯量 矩形转动惯量 J=(1/12)*m*(a^2+b^2)
	//wt_r32 I;
} wt_rect;

typedef struct {
	void *shape;
	wt_i32 type; //类型
} wt_shape;

static wt_shape *wt_create_shape(void *shape, wt_i32 type) {
	wt_shape *s = NULL;
	s = (wt_shape *) malloc(sizeof(wt_shape));
	s->shape = shape;
	s->type = type;
	return s;
}

static wt_circle *wt_create_cir(wt_body *body, wt_r32 radius) {
	wt_circle *c = NULL;
	c = (wt_circle *) malloc(sizeof(wt_circle));
	c->body   = body;
	c->radius = radius;
	body->I   = (body->mas == WT_MAX_R32) ? WT_MAX_R32 : body->mas * radius * radius / 2;
	return c;
}

static wt_rect *wt_create_rect(wt_body *body, wt_r32 u_len, wt_r32 r_len) {
	wt_rect *r = NULL;
	r = (wt_rect *) malloc(sizeof(wt_rect));
	r->body    = body;
	r->u_len   = u_len;
	r->r_len   = r_len;
	//矩形转动惯量 矩形转动惯量 J=(1/12)*m*(a^2+b^2)
	body->I = (body->mas == WT_MAX_R32) ? WT_MAX_R32 : body->mas * (4 * u_len * u_len + 4 * r_len * r_len) / 12;
	return r;
}

static wt_body *wt_shape_get_body(wt_shape *shape) {
	wt_body* b;
	switch (shape->type) {
	case WT_CIR:
		b = ((wt_circle*) (shape->shape))->body;
		break;
	case WT_REC:
		b = ((wt_rect*) (shape->shape))->body;
		break;
	}
	return b;
}

/**
 * 计算矩形上与给定点p 最近的点
 */
static wt_vec wt_closest_p_to_rect(wt_rect r,wt_vec p){

	wt_vec u1 = wt_v(0,r.u_len); u1 = wt_vrotation(u1,r.body->angular); u1 = wt_vunit(u1);
	wt_vec u2 = wt_v(r.r_len,0); u2 = wt_vrotation(u2,r.body->angular); u2 = wt_vunit(u2);

	wt_vec d = wt_vsub(p,r.body->pos);
	wt_vec q = r.body->pos;

	wt_r32 dist;
	dist = wt_rclamp(wt_vdot(d,u1),-r.u_len,r.u_len);
	q = wt_vadd(q,wt_vmuls(u1,dist));

	dist = wt_rclamp(wt_vdot(d,u2),-r.r_len,r.r_len);
	q = wt_vadd(q,wt_vmuls(u2,dist));

	return q;
}
/**
 * p 是否在矩形内
 * a1 为矩形左上角点,顺时针一次为 a2 a3 a4
 */
static wt_status wt_is_p_in_rect(wt_vec p,wt_vec a1,wt_vec a2,wt_vec a3,wt_vec a4){
	wt_vec a12 = wt_vsub(a1,a2);wt_vec a43 = wt_vsub(a4,a3);
	wt_vec a14 = wt_vsub(a1,a4);wt_vec a23 = wt_vsub(a2,a3);

	wt_vec a1p = wt_vsub(a1,p);wt_vec a4p = wt_vsub(a4,p);
	wt_vec a2p = wt_vsub(a2,p);
	if(wt_vmulv(a1p,a12) * wt_vmulv(a4p,a43)<=0 && wt_vmulv(a1p,a14) * wt_vmulv(a2p,a23)<=0){
		//printf("!!!mul1: %f \n",wt_vmulv(a1p,a12) * wt_vmulv(a4p,a34));
		//printf("!!!mul2: %f \n",wt_vmulv(a1p,a14) * wt_vmulv(a2p,a23));
		return 1;
	}else{
		return 0;
	}
}

/**
 * 矩形转为 四个点的形式
 */
static wt_status wt_rect_to_4p(wt_rect r,wt_vec *a1,wt_vec *a2,wt_vec *a3,wt_vec *a4){
	wt_vec u1 = wt_v(0,r.u_len); u1 = wt_vrotation(u1,r.body->angular);
	wt_vec u2 = wt_v(r.r_len,0); u2 = wt_vrotation(u2,r.body->angular);

	wt_vec a1p = wt_vadd(u1,wt_nvec(u2));
	wt_vec a2p = wt_vadd(u1,u2);
	wt_vec a3p = wt_vadd(wt_nvec(u1),u2);
	wt_vec a4p = wt_vadd(wt_nvec(u1),wt_nvec(u2));

	a1->x = a1p.x + r.body->pos.x;a1->y = a1p.y + r.body->pos.y;
	a2->x = a2p.x + r.body->pos.x;a2->y = a2p.y + r.body->pos.y;
	a3->x = a3p.x + r.body->pos.x;a3->y = a3p.y + r.body->pos.y;
	a4->x = a4p.x + r.body->pos.x;a4->y = a4p.y + r.body->pos.y;
}

#endif

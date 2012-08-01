/*
 * wt_shape_collision.h
 *
 *  Created on: 2012-2-21
 *      Author: vvilp
 */

#ifndef WT_SHAPE_COLLISION_H_
#define WT_SHAPE_COLLISION_H_
#include "wt_shape.h"
#define WT_SEP 0 //分离
#define WT_CLS 1 //碰撞
#define WT_PEN 2 //穿过

typedef struct {
    wt_vec contact_p;//碰撞点
    wt_body *b1;
    wt_body *b2;
    wt_vec aply_uv; //作用线单位向量 b 指向 a
    wt_r32 e;//碰撞系数
    wt_r32 u;//摩擦系数

}wt_contact;

//typedef struct {
//  wt_shape *a;
//  wt_shape *b;
//  wt_r32 e;
//  //碰撞系数
//  wt_r32 u;
//  //摩擦系数
//
//  wt_array *contacts;
//} wt_collision;


static wt_status wt_clear_array_contact(wt_array *contacts){
    for(int i = 0 ; i < contacts->num ;i++){
        wt_contact *c = contacts->array[i];
        free(c);
    }
    wt_array_clear(contacts);
}

static wt_contact* wt_create_contact(wt_body *b1,wt_body *b2,
                                        wt_vec contact_p,wt_vec aply_uv,
                                        wt_r32 e,wt_r32 u ){
    wt_contact* c = (wt_contact*) malloc (sizeof(wt_contact));
    c->b1 = b1;
    c->b2 = b2;
    c->e = e;
    c->u = u;
    c->aply_uv = aply_uv;
    c->contact_p = contact_p;
    return c;
}

//检测圆是否碰撞 array_c返回碰撞点array
static wt_status wt_cir2cir(wt_circle *c1, wt_circle *c2, wt_array *array_c) {
    wt_body *b1 = c1->body;
    wt_body *b2 = c2->body;
    wt_r32 r1 = c1->radius;
    wt_r32 r2 = c2->radius;
    wt_r32 d = (b1->pos.x - b2->pos.x) * (b1->pos.x - b2->pos.x)
            +  (b1->pos.y - b2->pos.y) * (b1->pos.y - b2->pos.y)
            -  (c1->radius + c2->radius) * (c1->radius + c2->radius);
    wt_vec c_pos;
    if (d <= 0.0001) {
        c_pos.x = (r1 * b2->pos.x + r2 * b1->pos.x) / (r1 + r2);
        c_pos.y = (r1 * b2->pos.y + r2 * b1->pos.y) / (r1 + r2);

        wt_vec aply_uv = wt_vunit(wt_vsub(b1->pos,c_pos));
        wt_contact *contact = wt_create_contact(b1,b2,c_pos,aply_uv,1,0.5);

        wt_array_add(array_c,contact);
        return WT_CLS;
    }
    return WT_SEP;
}

//检测圆矩形是否碰撞 array_c返回碰撞点array
static wt_status wt_cir2rec(wt_circle *cir, wt_rect *rec, wt_array *array_c) {
    //计算与圆最近点
    wt_vec closest_p = wt_closest_p_to_rect(*rec, cir->body->pos);
    wt_r32 d1 = (closest_p.x - cir->body->pos.x) * (closest_p.x - cir->body->pos.x)
              + (closest_p.y - cir->body->pos.y) * (closest_p.y - cir->body->pos.y);
    wt_r32 d2 = cir->radius * cir->radius;
    if (d1-d2 <= 0.0f)
    {
        wt_vec aply_uv = wt_vunit(wt_vsub(cir->body->pos,closest_p));
        wt_contact *contact = wt_create_contact(cir->body,rec->body,closest_p,aply_uv,1,0.5);
        wt_array_add(array_c,contact);
        return WT_CLS;
    }
    return WT_SEP;
}

static wt_vec wt_p_vec_aply(wt_vec p,wt_vec a,wt_vec b,wt_vec pos){
    wt_vec v = wt_v(0,0);
    if(wt_fabs(wt_vmulv(wt_vsub(a,p),wt_vsub(b,p)))<1){
        v = wt_vperp(wt_vsub(a,b));
        if(wt_vmulv(wt_vsub(pos,p),v)<0) v = wt_nvec(v);
    }
    return v;
}

//当p在矩形上或矩形内,求p作用线向量单位 与 pos 同向
static wt_vec wt_p_aply_uv(wt_vec p,wt_vec a1,wt_vec a2,wt_vec a3,wt_vec a4,wt_vec pos){
    wt_vec v;
    v = wt_p_vec_aply(p, a1, a2, pos);
    if (!wt_v_is_0(v)){
        printf("a1,a2\n");
        return v;
    }
    v = wt_p_vec_aply(p, a2, a3, pos);
    if (!wt_v_is_0(v)){
        printf("a2,a3\n");
        return v;
    }

    v = wt_p_vec_aply(p, a3, a4, pos);
    if (!wt_v_is_0(v)){
        printf("a3,a4\n");
        return v;
    }

    v = wt_p_vec_aply(p, a4, a1, pos);
    if (!wt_v_is_0(v)){
        printf("a4,a1\n");
        return v;
    }


    return v;
}
static wt_status wt_rec2rec(wt_rect *rec1, wt_rect *rec2, wt_array *array_c) {
    wt_vec r1p[4] , r2p[4];

    wt_rect_to_4p(*rec1,&r1p[0],&r1p[1],&r1p[2],&r1p[3]);
    wt_rect_to_4p(*rec2,&r2p[0],&r2p[1],&r2p[2],&r2p[3]);

    int status = WT_SEP;
    for(int i = 0 ; i < 4;i++){
        if(wt_is_p_in_rect(r1p[i],r2p[0],r2p[1],r2p[2],r2p[3])){
            wt_vec aply_uv = wt_nvec(wt_p_aply_uv(r1p[i],r2p[0],r2p[1],r2p[2],r2p[3],rec2->body->pos));
            aply_uv = wt_vunit( aply_uv);
            debug_vec(aply_uv,"起始aply_uv");
            wt_contact *contact = wt_create_contact(rec1->body,rec2->body,r1p[i],aply_uv,1,0.5);
            wt_array_add(array_c,contact);
            status = WT_CLS;
        }
    }
//
    for(int i = 0 ; i < 4;i++){
        if(wt_is_p_in_rect(r2p[i],r1p[0],r1p[1],r1p[2],r1p[3])){
            wt_vec aply_uv = wt_p_aply_uv(r2p[i],r1p[0],r1p[1],r1p[2],r1p[3],rec1->body->pos);
            aply_uv = wt_vunit( aply_uv);
            debug_vec(aply_uv,"起始aply_uv");
            wt_contact *contact = wt_create_contact(rec1->body,rec2->body,r2p[i],aply_uv,1,0.5);
            wt_array_add(array_c,contact);
            status = WT_CLS;
        }
    }

    return status;
}
typedef wt_status (*collision_funs)(const void *, const void *, wt_array *);
static const collision_funs wt_cls_funs[10] = {
    NULL,
    wt_cir2cir,
    wt_rec2rec,
    wt_cir2rec
};

static wt_status wt_shape2shape(wt_shape s1, wt_shape s2,wt_array *array_c){
    collision_funs cls_f = wt_cls_funs[s1.type | s2.type];

    wt_i32 ret = 0;
    //wt_vec c_pos;
    if(s1.type > s2.type){
        ret = cls_f(s2.shape,s1.shape,array_c);
    }else{
        ret = cls_f(s1.shape,s2.shape,array_c);
    }

    //contact->contact_p = c_pos;
    //contact->
    return ret;

}

#endif /* WT_SHAPE_COLLISION_H_ */

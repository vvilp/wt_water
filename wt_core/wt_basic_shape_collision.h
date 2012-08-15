#ifndef _WT_BASIC_SHAPE_COLLISION_
#define _WT_BASIC_SHAPE_COLLISION_

#include "wt_core.h"
typedef struct
{
    wt_body *b1;
    wt_body *b2;
    wt_r32 impulse_n;
    wt_r32 impulse_t;
    wt_r32 bias;
    //box2d 方法用来修正重叠
    wt_r32 separation;
    //distance of sep ,if penetration, is  negative
    wt_r32 allow_penetration;
    //允许的贯穿量
    wt_r32 restitution;
    // 区分弹性 非弹性 碰撞
    wt_vec normal;
    //wt_vec tangent;
    wt_r32 k_normal;
    wt_r32 k_tangent;
    wt_vec pos;

} wt_contact;

static wt_contact *wt_create_contact(wt_body *b1, wt_body *b2, wt_vec normal, wt_r32 separation,
                                     wt_r32 allow_penetration, wt_r32 restitution, wt_vec pos)
{
    wt_contact *contact = (wt_contact *) malloc (sizeof(wt_contact));
    contact->b1 = b1;
    contact->b2 = b2;
    contact->restitution        = restitution;
    contact->normal             = normal;
    contact->allow_penetration  = allow_penetration;
    contact->separation         = separation;
    contact->impulse_n          = 0;
    contact->impulse_t          = 0;
    contact->bias               = 0;
    contact->k_normal           = 0;
    contact->k_tangent          = 0;
    contact->pos                = pos;
    return contact;
}

//test does 2 cir collide
static wt_status wt_test_cir2cir_collision(wt_r32 posx1, wt_r32 posy1, wt_r32 radius1,
        wt_r32 posx2, wt_r32 posy2, wt_r32 radius2,
        wt_r32 epsilon)
{
    wt_r32 dx = (posx1 - posx2);
    wt_r32 dy = (posy1 - posy2);
    //wt_r32 distance = (wt_r32)wt_sqrt(dx * dx + dy * dy) - epsilon;
    return (wt_r32) wt_sqrt(dx * dx + dy * dy) - epsilon < (radius1 + radius2);
}

//在碰撞的情况下，计算contact
static wt_status wt_cir2cir_contact(wt_circle *c1, wt_circle *c2, wt_array *contact_array)
{
    wt_body *b1, *b2;
    b1 = c1->body; b2 = c2->body;
    wt_r32 dx = b1->pos.x - b2->pos.x;
    wt_r32 dy = b1->pos.y - b2->pos.y;
    wt_r32 dis = (wt_r32) wt_sqrt((dx * dx + dy * dy));
    wt_vec contact_pos = wt_v( (b1->pos.x + b2->pos.x) / 2, (b1->pos.y + b2->pos.y) / 2 );
    wt_r32 separation = dis - c1->radius - c2->radius;
    wt_vec normal = wt_vsub(b2->pos, b1->pos);
    normal = wt_vunit(normal);
    wt_contact *contact = wt_create_contact(b1, b2, normal, separation, 0.001f, (b1->restitution + b2->restitution) / 2, contact_pos);
    wt_array_add(contact_array, contact);
}

static wt_status wt_cir2cir_collsion(wt_circle *c1, wt_circle *c2, wt_array *contact_array)
{
    wt_body *b1 = c1->body;
    wt_body *b2 = c2->body;
    if (wt_test_cir2cir_collision(b1->pos.x, b1->pos.y, c1->radius, b2->pos.x, b2->pos.y, c2->radius, 1e-5f))
    {
        wt_cir2cir_contact(c1, c2, contact_array);
    }


}
//-----------------------------------------------------------------------------------------------------------------------------

static wt_status wt_test_cir2point_collision(wt_vec cir_pos, wt_r32 radius, wt_vec point_pos,wt_r32 epsilon)
{
    wt_r32 dx = (cir_pos.x - point_pos.x);
    wt_r32 dy = (cir_pos.y - point_pos.y);
    //wt_r32 distance = (wt_r32)wt_sqrt(dx * dx + dy * dy) - epsilon;
    return (wt_r32) wt_sqrt(dx * dx + dy * dy) - epsilon < radius;
}

static wt_status wt_cir2fluid_partical_contact(wt_circle *c1, wt_pvf_partical *p1, wt_array *contact_array)
{
    wt_body *b1, *b2;
    b1 = c1->body; 
    b2 = p1->body;
    wt_r32 dx = b1->pos.x - b2->pos.x;
    wt_r32 dy = b1->pos.y - b2->pos.y;
    wt_r32 dis = (wt_r32) wt_sqrt((dx * dx + dy * dy));
    wt_vec contact_pos = p1->body->pos;
    wt_r32 separation = dis - c1->radius;
    wt_vec normal = wt_vsub(b2->pos, b1->pos);
    normal = wt_vmuls(normal, 1.0 / dis);
    wt_contact *contact = wt_create_contact(b1, b2, normal, separation, 0.001f, (b1->restitution + b2->restitution) / 2, contact_pos);
    wt_array_add(contact_array, contact);
}

static wt_status wt_cir2fluid_partical_collision(wt_circle *c1,wt_pvf_partical *p1,wt_array *contact_array)
{
    if(wt_test_cir2point_collision(c1->body->pos, c1->radius, p1->body->pos, 1e-5f))
    {
        wt_cir2fluid_partical_contact(c1, p1, contact_array);
    }
}

//检测碰撞，如果碰撞 则加入碰撞表中
static wt_status wt_shape2shape_collision(wt_shape *s1, wt_shape *s2, wt_array *contact_array)
{
    if (s1->type == WT_CIR)
    {
        wt_circle *c1 = (wt_circle *)s1->shape;
        if (s2->type == WT_CIR)
        {
            wt_circle *c2 = (wt_circle *)s2->shape;
            wt_cir2cir_collsion(c1, c2, contact_array);
        }
    }

}

static wt_status wt_shape2fluid_partical_collision(wt_shape *s1, wt_pvf_partical *p1, wt_array *contact_array)
{
    if (s1->type == WT_CIR)
    {
        wt_circle *c1 = (wt_circle *)s1->shape;
        wt_cir2fluid_partical_collision(c1,p1,contact_array);
    }
}


#endif
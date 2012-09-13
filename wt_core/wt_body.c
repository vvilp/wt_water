#include "wt_core.h"

wt_body *wt_create_body(wt_r32 mas, wt_vec pos, wt_vec ael,
                        wt_vec vel, wt_r32 ang_ael, wt_r32 ang_vel,
                        wt_r32 angular)
{
    wt_body *b = NULL;
    b = (wt_body *)malloc(sizeof(wt_body));
    b -> mas = mas;
    b->inv_mas = (mas == WT_MAX_R32) ?  0 : ((mas == 0) ? WT_MAX_R32 : 1 / mas);
    b -> pos.x = pos.x; b -> pos.y = pos.y;
    b -> pre_pos = pos;
    b -> vel.x = vel.x; b -> vel.y = vel.y;
    b -> ael.x = ael.x; b -> ael.y = ael.y;
    b -> ang_ael = ang_ael; b -> ang_vel = ang_vel; b -> angular = angular;
    
    b -> I = 0;
    b->inv_I = WT_MAX_R32;
    b -> force = wt_v(0,0);
    b->restitution = 0.2;
    b -> fric = 0.5;

    return b;
}

wt_body *wt_create_body0(wt_r32 mas, wt_vec pos, wt_r32 angular)
{
    return wt_create_body(mas, pos, wt_v(0, 0), wt_v(0, 0), 0, 0, angular);
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
    if (b == NULL) return WT_ER;
    wt_vector pos, vel, ael;
    pos = b -> pos; vel = b -> vel; ael = b -> ael;
    pos.x += vel.x; pos.y += vel.y;
    vel.x += ael.x; vel.y += ael.y;
    b -> pos = pos; b -> vel = vel; b->ael = ael;
    return WT_OK;
}

//当角度大于2bi 或 小于 -2bi 重置角度
wt_r32 wt_reset_angular(wt_r32 a)
{
    if (wt_cmp_real(a, WT_PI * 2) == 1)     return a - WT_PI * 2;
    if (wt_cmp_real(a, -WT_PI * 2) == -1)   return a + WT_PI * 2;
    return a;
}
wt_status wt_body_update_step(wt_body *b, float wt_dtime)
{
    if (b == NULL) return WT_ER;

    wt_r32 a_ael = b -> ang_ael;
    wt_r32 a_vel = b -> ang_vel;
    wt_r32 ang   = b -> angular;

    ang = ang + a_vel * wt_dtime;
    ang = wt_reset_angular(ang);
    a_vel += a_ael * wt_dtime;

    b -> ang_ael = a_ael; b -> ang_vel = a_vel; b -> angular = ang;

    b->pre_pos = b->pos;
    b->vel = wt_vadd(b->vel, wt_vmuls(b->ael, wt_dtime));
    b->pos = wt_vadd(b->pos, wt_vmuls(b->vel, wt_dtime));

    //wt_debug("pos %f %f \n", b->pos.x,b->pos.y);

    return WT_OK;
}

void wt_body_collide_border(wt_body *bi)
{
    if (bi->pos.x  < 5 )
    {
        bi->pos.x  = 5 ;
        bi->vel.x = - bi->vel.x * 0.8;
    }

    if (bi->pos.x > 95 )
    {
        bi->pos.x  = 95 ;
        bi->vel.x = -bi->vel.x * 0.8;
    }

    if (bi->pos.y  < 5)
    {
        bi->pos.y = 5 ;
        bi->vel.y = -bi->vel.y * 0.8;
    }

    if (bi->pos.y > 95 )
    {
        bi->pos.y = 95  ;
        bi->vel.y = -bi->vel.y * 0.8;
    }
}

void wt_body_restrict_vel(wt_body *bi, wt_r32 max_vel)
{
    bi->vel.x = (bi->vel.x > max_vel) ? max_vel : bi->vel.x;
    bi->vel.y = (bi->vel.y > max_vel) ? max_vel : bi->vel.y;
    bi->vel.x = (bi->vel.x < -max_vel) ? -max_vel : bi->vel.x;
    bi->vel.y = (bi->vel.y < -max_vel) ? -max_vel : bi->vel.y;
}

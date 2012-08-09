#include "wt_core.h"

wt_partical *wt_create_partical(wt_r32 mas,wt_r32 radius, wt_vec pos, wt_vec vel, wt_vec ael)
{
    wt_partical *p = NULL;

    p = (wt_partical *) malloc (sizeof(wt_partical));
    p -> mas = mas;

    p->inv_mas = (mas == WT_MAX_R32) ?  0 : ((mas == 0) ? WT_MAX_R32 : 1 / mas);

    p->radius = radius;
    p->ael = ael;
    p->vel = vel;
    p->pos = pos;
    return p;

}

void wt_partical_restrict_vel(wt_partical *pi, wt_r32 max_vel)
{
    pi->vel.x = (pi->vel.x > max_vel) ? max_vel : pi->vel.x;
    pi->vel.y = (pi->vel.y > max_vel) ? max_vel : pi->vel.y;
    pi->vel.x = (pi->vel.x < -max_vel) ? -max_vel : pi->vel.x;
    pi->vel.y = (pi->vel.y < -max_vel) ? -max_vel : pi->vel.y;
}

void wt_partical_collide_border(wt_partical *pi)
{
    if (pi->pos.x < 5 )
    {
        pi->pos.x = 5;
        pi->vel.x = - pi->vel.x *0.8;
    }

    if (pi->pos.x > 95 )
    {
        pi->pos.x = 95;
        pi->vel.x = - pi->vel.x *0.8;
    }

    if (pi->pos.y < 5)
    {
        pi->pos.y = 5;
        pi->vel.y = - pi->vel.y *0.8;
    }

    if (pi->pos.y > 95)
    {
        pi->pos.y = 95;
        pi->vel.y = - pi->vel.y *0.8;
    }
}

wt_status wt_partical_update(wt_partical *p, wt_r32 dt)
{
	if (p == NULL) return WT_ER;

    p->pre_pos = p->pos;
	p->pos = wt_vadd(p->pos, wt_vmuls(p->vel, dt));
	//p->vel = wt_vadd(p->vel, wt_vmuls(p->ael, dt));

    //if(wt_vlen2(p->vel) > 100.0){
    //    p->vel = wt_vmuls(p->vel, 0.5);
    //}

	//wt_debug("p->pos x:%f, y:%f \n", p->pos.x,p->pos.y);
	//wt_debug("p->vel x:%f, y:%f \n", p->vel.x,p->vel.y);
    //system("pause");

	return WT_OK;
}
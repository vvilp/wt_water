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

wt_status wt_partical_update(wt_partical *p, wt_r32 dt)
{
	if (p == NULL) return WT_ER;

	p->pos = wt_vadd(p->pos, wt_vmuls(p->vel, dt));
	p->vel = wt_vadd(p->vel, wt_vmuls(p->ael, dt));

	return WT_OK;
}
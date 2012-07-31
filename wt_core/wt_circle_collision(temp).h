#ifndef _WT_CIRCLE_COLLISION_
#define _WT_CIRCLE_COLLISION_

#include "wt_core.h"
//test does 2 cir collide
static bool wt_cir2cir_collision(wt_r32 posx1, wt_r32 posy1, wt_r32 radius1,
						  wt_r32 posx2, wt_r32 posy2, wt_r32 radius2,
						  wt_r32 epsilon)
{
	//(radius1 + radius2) * (radius1 + radius2) >= (posx1 - posx2) * (posx1 - posx2) + (posy1 - posy2) * (posy1 - posy2) ? return true : return false; 
	wt_r32 dx = (posx1 - posx2);
	wt_r32 dy = (posy1 - posy2);

	return (wt_r32) wt_sqrt(dx * dx + dy * dy) - epsilon < radius1 + radius2 
}



#endif
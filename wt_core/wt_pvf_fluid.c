#include "wt_core.h"

wt_pvf_partical *wt_create_pvf_partical(wt_partical * p)
{
	wt_pvf_partical *pvf_p = (wt_pvf_partical *) malloc (sizeof(wt_pvf_partical));
	pvf_p -> partical = p;
	pvf_p -> p_density = 0;
	return pvf_p;
}
wt_pvf_fluid *wt_create_pvf_fluid()
{
	wt_pvf_fluid *f = (wt_pvf_fluid *) malloc (sizeof(wt_pvf_fluid));
	f->density = 0;
	f->pvf_particals = wt_array_init(100);
	return f;
}
void wt_pvf_add_partical(wt_pvf_fluid *f, wt_pvf_partical* p)
{
	wt_array_add(f->pvf_particals, p);
}
void wt_pvf_update_fluid(wt_pvf_fluid *f, wt_r32 dt)
{

}
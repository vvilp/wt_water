#include "wt_core.h"

wt_sph_partical* wt_create_sph_partical(wt_partical * partical)
{
	wt_sph_partical *p = (wt_sph_partical*) malloc (sizeof(wt_sph_partical));

	p->partical = partical;
	p->p_density = 0.0;
	p->ael_pressure = wt_v(0,0);
	p->ael_viscosity = wt_v(0,0);
	return p;
}

wt_sph_fluid* wt_create_sph_fluid(wt_r32 density, wt_r32 viscosity)
{
	wt_sph_fluid* f = (wt_sph_fluid*) malloc (sizeof(wt_sph_fluid));
	f->sph_particals = wt_array_init(100);
	f->density = density;
	f->viscosity = viscosity;
	return f;
}

void wt_sph_add_partical(wt_sph_fluid *fluid, wt_sph_partical *sp)
{
	wt_debug("add fluid partical\n",1);
	wt_array_add(fluid->sph_particals, sp);
}

void wt_sph_update_density(wt_sph_fluid *fluid)
{

}

void wt_sph_update_ael_pressure(wt_sph_fluid *fluid)
{

}

void wt_sph_update_ael_viscosity(wt_sph_fluid *fluid)
{

}

void wt_sph_update_partical(wt_sph_fluid *fluid)
{

}

void wt_sph_update_fluid(wt_sph_fluid *fluid, wt_r32 dt)
{
	wt_debug("update_fluid begin\n",1);

	wt_sph_update_density(fluid);

	wt_sph_update_ael_pressure(fluid);

	wt_sph_update_ael_viscosity(fluid);

	wt_sph_update_partical(fluid);
}
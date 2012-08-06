#include "wt_core.h"

wt_sph_partical *wt_create_sph_partical(wt_partical *partical)
{
    wt_sph_partical *p = (wt_sph_partical *) malloc (sizeof(wt_sph_partical));
    p->partical = partical;

    //计算后更新
    p->p_density = 0.0;
    p->p_density_near = 0.0;
    p->ael_pressure = wt_v(0, 0);
    p->press = 0.0;
    p->press_near = 0.0;
    p->ael_viscosity = wt_v(0, 0);
    return p;
}

wt_sph_fluid *wt_create_sph_fluid(wt_r32 density, wt_r32 viscosity, wt_r32 k, wt_r32 h)
{
    wt_sph_fluid *f = (wt_sph_fluid *) malloc (sizeof(wt_sph_fluid));
    f->sph_particals = wt_array_init(100);
    f->density = 1000;
    f->viscosity_beta = 0.2;
    f->viscosity_sigma = 0.2;
    f->h = 19.5198;
    f->k = 0.112345;
    return f;
}

void wt_sph_add_partical(wt_sph_fluid *fluid, wt_sph_partical *sp)
{
    wt_array_add(fluid->sph_particals, sp);
}

//----------------------------------------------

void wt_sph_update_density(wt_sph_fluid *fluid)
{
    wt_array *sps = fluid->sph_particals;
    wt_r32 h = fluid->h;
    for (int i = 0; i < sps->num ; i++)
    {
        wt_sph_partical *spi = sps->array[i];
        wt_partical *pi = spi->partical;
        wt_r32 sum = 0.0;
        for (int j = i + 1 ; j < sps->num ; j++)
        {
            wt_sph_partical *spj = sps->array[j];
            wt_partical *pj = spj->partical;
            wt_r32 len2 = wt_vlen2(wt_vsub(pi->pos, pj->pos));
            if (len2 < h * h)
            {
                wt_r32 len = wt_sqrt(len2);
                wt_r32 q = 1.0 -  len / fluid->h;
                wt_r32 q2 = q * q;
                wt_r32 q3 = q2 * q;
                spi->p_density += q2;
                spi->p_density_near += q3;
                spj->p_density += q2;
                spj->p_density_near += q3;
            }
        }
    }
}

void wt_sph_update_ael_pressure(wt_sph_fluid *fluid)
{
    wt_array *sps = fluid->sph_particals;
    wt_r32 h = fluid->h;
    wt_r32 k = fluid->k;

    for (int i = 0; i < sps->num ; i++)
    {
        wt_sph_partical *spi = sps->array[i];
        wt_partical *pi = spi->partical;
        spi->press = k * (spi->p_density - fluid->density);
        spi->press_near = k * 5 * spi->p_density ;
    }

    for (int i = 0; i < sps->num ; i++)
    {
        wt_sph_partical *spi = sps->array[i];
        wt_partical *pi = spi->partical;

        for (int j = i + 1 ; j < sps->num ; j++)
        {
            wt_sph_partical *spj = sps->array[j];
            wt_partical *pj = spj->partical;
            wt_vec pij = wt_vsub(pi->pos, pj->pos);
            wt_r32 len2 = wt_vlen2(pij);
            if (len2 < h * h)
            {
                wt_r32 len = wt_sqrt(len2);
                //wt_r32 press = k * (spi->p_density + spj->p_density - 2 * fluid->density);
                // wt_vec f_presssure  = wt_vmuls(pij, (press / len * 0.5));
                // spi->ael_pressure = wt_vsub(spi->ael_pressure, f_presssure);
                // spj->ael_pressure = wt_vadd(spj->ael_pressure, f_presssure);
                wt_r32 dm = spi->press + spj->press + spi->p_density_near + spj->p_density_near;
                wt_vec D = wt_vmuls(wt_vunit(pij), dm);
                spi->ael_pressure = wt_vsub(spi->ael_pressure, D);
                spj->ael_pressure = wt_vadd(spj->ael_pressure, D);
            }
        }
    }
}

void wt_sph_update_ael_viscosity(wt_sph_fluid *fluid)
{
    wt_array *sps = fluid->sph_particals;
    wt_r32 h = fluid->h;
    for (int i = 0; i < sps->num ; i++)
    {
        wt_sph_partical *spi = sps->array[i];
        wt_partical *pi = spi->partical;
        for (int j = i + 1 ; j < sps->num ; j++)
        {
            wt_sph_partical *spj = sps->array[j];
            wt_partical *pj = spj->partical;
            wt_vec pij = wt_vsub(pi->pos, pj->pos);
            wt_r32 len2 = wt_vlen2(pij);
            if (len2 < h * h)
            {
                wt_r32 len = wt_sqrt(len2);
                wt_vec pij_normal = wt_vunit(pij);
                wt_vec dv = wt_vsub(pi->vel, pj->vel);
                wt_r32 dv_scalar = 0.0; //dv在方向向量的投影，既相对速度大小
                dv_scalar = wt_vdot(dv, pij_normal);
                // wt_debug("dv_scalar %f\n",dv_scalar);
                //wt_debug("dv %d\n",dv_scalar);
                //wt_debug("dv_scalar %d\n",dv_scalar);
                //system("pause");
                if (dv_scalar > 0)
                {
                    wt_r32 q = len / pj->radius;
                    wt_r32 impulse =  (1 - q) * (fluid->viscosity_sigma * dv_scalar + fluid->viscosity_beta * dv_scalar * dv_scalar);
                    // wt_debug("(1 - q) %f\n",(1 - q));
                    // wt_debug("fluid->viscosity_sigma * dv_scalar %f\n",fluid->viscosity_sigma * dv_scalar);
                    // wt_debug("fluid->viscosity_beta*dv_scalar*dv_scalar %f\n",fluid->viscosity_beta*dv_scalar*dv_scalar);
                    // wt_debug("impulse %f\n",impulse);

                    wt_vec v_impulse = wt_vmuls(pij_normal, impulse / 2); // /2 表示 质量都为1
                    //spi->ael_viscosity = wt_vsub(spi->ael_viscosity, v_impulse);
                    //spj->ael_viscosity = wt_vadd(spj->ael_viscosity, v_impulse);
                    // wt_debug("v_impulse %f %f\n",v_impulse.x,v_impulse.y);
                    pi->vel = wt_vsub(pi->vel, v_impulse);
                    pj->vel = wt_vadd(pj->vel, v_impulse);

                    // wt_debug("pi->vel %f %f\n",pi->vel.x,pi->vel.y);
                    // wt_debug("pj->vel %f %f\n",pj->vel.x,pj->vel.y);
                    //system("pause");
                }
            }

        }
    }
}

void wt_sph_update_partical(wt_sph_fluid *fluid, wt_r32 dt)
{
    wt_array *sps = fluid->sph_particals;
    for (int i = 0; i < sps->num ; i++)
    {
        wt_sph_partical *spi = sps->array[i];
        wt_partical *pi = spi->partical;

        // wt_debug("pos:%d x:%f, y:%f \n", i, pi->pos.x, pi->pos.y);
        // wt_debug("vel:%d x:%f, y:%f \n", i, pi->vel.x, pi->vel.y);
        // wt_debug("ael:%d x:%f, y:%f \n", i, pi->ael.x, pi->ael.y);
        // wt_debug("ael_pressure:%d  x:%f, y:%f \n", i, spi->ael_pressure.x, spi->ael_pressure.y);
        // wt_debug("ael_viscosity:%d  x:%f, y:%f \n", i, spi->ael_viscosity.x, spi->ael_viscosity.y);

        pi->ael = wt_vadd(wt_v(0, -10.0), spi->ael_pressure);
        //pi->ael = wt_vadd(pi->ael, spi->ael_viscosity);

        wt_partical_update(pi, dt);
    }
}

//临时
wt_status wt_sph_collide_border(wt_sph_fluid *fluid)
{
    wt_array *sps = fluid->sph_particals;
    for (int i = 0; i < sps->num ; i++)
    {
        wt_sph_partical *spi = sps->array[i];
        wt_partical *pi = spi->partical;

        if (pi->pos.y < 0)
        {
            pi->vel.y = - pi->vel.y;
        }

        if (pi->pos.y > 300)
        {
            pi->vel.y = - pi->vel.y;
        }

    }
}

void wt_sph_update_fluid(wt_sph_fluid *fluid, wt_r32 dt)
{
    //wt_debug("update_fluid begin\n",1);
    //wt_sph_init();

    //wt_sph_update_density(fluid);

    //wt_sph_update_ael_pressure(fluid);

    //wt_sph_update_ael_viscosity(fluid);

    wt_sph_update_partical(fluid, dt);

    wt_sph_collide_border(fluid);
}
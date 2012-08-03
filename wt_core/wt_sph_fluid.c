#include "wt_core.h"

wt_sph_partical *wt_create_sph_partical(wt_partical *partical)
{
    wt_sph_partical *p = (wt_sph_partical *) malloc (sizeof(wt_sph_partical));

    p->partical = partical;

    //计算后更新
    p->p_density = 0.0;
    p->ael_pressure = wt_v(0, 0);
    p->ael_viscosity = wt_v(0, 0);
    //p->h = partical->radius*3; //作用范围是粒子半径3倍
    return p;
}

wt_sph_fluid *wt_create_sph_fluid(wt_r32 density, wt_r32 viscosity, wt_r32 k, wt_r32 h)
{
    wt_sph_fluid *f = (wt_sph_fluid *) malloc (sizeof(wt_sph_fluid));
    f->sph_particals = wt_array_init(100);
    f->density = density;
    f->viscosity = viscosity;
    f->h = h;
    f->k = k;

    //初始化 核系数;
    //使用3d模型用来测试
    //3D:f->k_poly6 = 315.0 / (64 * WT_PI * wt_rpow(h, 9));
    //2D:
    f->k_poly6 = 4.0 / ( WT_PI * wt_rpow(h, 8));
    f->k_spiky = 45 / (WT_PI * wt_rpow(h, 6));
    f->k_viscosity = f->k_spiky;

    return f;
}

void wt_sph_add_partical(wt_sph_fluid *fluid, wt_sph_partical *sp)
{
    //wt_debug("add fluid partical\n", 1);
    wt_array_add(fluid->sph_particals, sp);
}

//----------------------------------------------

// void wt_sph_init(wt_sph_fluid *fluid)
// {

// }

void wt_sph_update_density(wt_sph_fluid *fluid)
{
    wt_array *sps = fluid->sph_particals;
    wt_r32 h = fluid->h;
    for (int i = 0; i < sps->num ; i++)
    {
        wt_sph_partical *spi = sps->array[i];
        wt_partical *pi = spi->partical;
        wt_r32 sum = 0.0;
        for (int j = 0 ; j < sps->num ; j++)
        {
            if (i == j)
            {
                sum += wt_rpow(h * h, 3);
            }
            wt_sph_partical *spj = sps->array[j];
            wt_partical *pj = spj->partical;
            wt_vec rirj = wt_vsub(pi->pos, pj->pos);
            wt_r32 len_rij = wt_vlen(rirj);
            sum += (len_rij < h) ? wt_rpow(h * h - len_rij * len_rij, 3) : 0.0;
        }
        spi->p_density = pi->mas * fluid->k_poly6 * sum;

        // wt_debug("spi->p_density:%f  \n", spi->p_density);
        // wt_debug("fluid->k_poly6:%f  \n", fluid->k_poly6);
        // wt_debug("sum:%f  \n", sum);
    }
}

void wt_sph_update_ael_pressure(wt_sph_fluid *fluid)
{
    wt_array *sps = fluid->sph_particals;
    wt_r32 h = fluid->h;
    wt_r32 k = fluid->k;

    //wt_debug("wt_sph_update_ael_pressure \n", 1);
    for (int i = 0; i < sps->num ; i++)
    {
        wt_sph_partical *spi = sps->array[i];
        wt_partical *pi = spi->partical;
        wt_r32 press_i = k * (spi->p_density - fluid->density);

        wt_vec sum = wt_v(0.0, 0.0);
        for (int j = 0 ; j < sps->num ; j++)
        {
            if (i == j) continue;
            wt_sph_partical *spj = sps->array[j];
            wt_partical *pj = spj->partical;
            wt_vec rirj = wt_vsub(pi->pos, pj->pos);
            wt_r32 len_rij = wt_vlen(rirj);
            wt_r32 press_j = k * (spj->p_density - fluid->density);
            if (len_rij < h)
            {
                wt_r32 sum_k = 0.0;
                sum_k = (press_i + press_j) / (2 * spi->p_density * spj->p_density);
                sum_k *= (h - len_rij) * (h - len_rij) / len_rij;
                sum = wt_vadd(sum, wt_vmuls(rirj, sum_k));
                //wt_debug("sum:  x:%f, y:%f \n", sum.x, sum.y);
                // if(i == 0 || i == 4){
                //     wt_debug("i:%d j:%d, sum_k: %f \n", i,j, sum_k);
                //     wt_debug("i:%d j:%d, sum: x:%f y:%f \n", i,j, sum.x,sum.y);
                //     _sleep(1000);
                // }
            }
        }
        spi->ael_pressure = wt_vmuls(sum, pi->mas * fluid->k_spiky);
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
        wt_vec sum = wt_v(0.0, 0.0);
        for (int j = 0 ; j < sps->num ; j++)
        {
            if (i == j) continue;
            wt_sph_partical *spj = sps->array[j];
            wt_partical *pj = spj->partical;
            wt_vec rirj = wt_vsub(pi->pos, pj->pos);
            wt_r32 len_rij = wt_vlen(rirj);
            wt_vec ui_uj = wt_vsub(pj->vel, pi->vel);
            if (len_rij < h)
            {
                wt_r32 sum_k = 0.0;
                sum_k = (h - len_rij) / (spi->p_density * spj->p_density);
                sum = wt_vadd(sum, wt_vmuls(ui_uj, sum_k));
            }
        }
        spi->ael_viscosity = wt_vmuls(sum, pi->mas * fluid->k_viscosity);
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

        pi->ael = wt_vadd(wt_v(0.0, 0.0), spi->ael_pressure);
        pi->ael = wt_vadd(pi->ael, spi->ael_viscosity);

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
        //pi->ael = wt_vmuls(pi->ael,spi->ael_pressure);
        //pi->ael = wt_vmuls(pi->ael,spi->ael_viscosity);
        if (pi->pos.y >= 100 || pi->pos.y <= -100)
            pi->vel.y = -pi->vel.y/3;
        if (pi->pos.x >= 100 || pi->pos.x <= -100)
            pi->vel.x = -pi->vel.x/3;

        // if (pi->pos.y >= 95) pi->pos.y = 95;
        // if (pi->pos.y <= -95) pi->pos.y = -95;
        // if (pi->pos.x >= 95) pi->pos.x = 95;
        // if (pi->pos.x <= -95) pi->pos.x = -95;
    }
}

void wt_sph_update_fluid(wt_sph_fluid *fluid, wt_r32 dt)
{
    //wt_debug("update_fluid begin\n",1);
    //wt_sph_init();

    wt_sph_update_density(fluid);

    wt_sph_update_ael_pressure(fluid);

    wt_sph_update_ael_viscosity(fluid);

    wt_sph_update_partical(fluid, dt);

    wt_sph_collide_border(fluid);
}
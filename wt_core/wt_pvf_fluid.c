#include "wt_core.h"

wt_pvf_partical *wt_create_pvf_partical(wt_partical *p)
{
    wt_pvf_partical *pvf_p = (wt_pvf_partical *) malloc (sizeof(wt_pvf_partical));
    pvf_p -> partical = p;
    pvf_p -> p_density = 0;
    pvf_p->p_density_near = 0;
    pvf_p->p_press = 0;
    pvf_p->p_press_near = 0;
    return pvf_p;
}
wt_pvf_fluid *wt_create_pvf_fluid()
{
    wt_pvf_fluid *f = (wt_pvf_fluid *) malloc (sizeof(wt_pvf_fluid));
    f->density = 10;
    f->h = 2 ;

    f->sigma = 0 ;
    f->beta = 0.3;
    f->k = 5;
    f->k_near = 2.5;
    f->pvf_particals = wt_array_init(100);
    f->partical_max_vel = 100.0;
    return f;
}
void wt_pvf_add_partical(wt_pvf_fluid *f, wt_pvf_partical *p)
{
    wt_array_add(f->pvf_particals, p);
}


void wt_pvf_viscosity_update_vel(wt_pvf_fluid *f, wt_r32 dt)
{
    wt_array *particals = f->pvf_particals;
    wt_r32 h = f->h;
    for (int i = 0 ; i < particals->num ; i++)
    {
        wt_pvf_partical *pvf_pi = particals->array[i];
        wt_partical *pi = pvf_pi->partical;
        for (int j = i + 1; j < particals->num ; j++)
        {
            wt_pvf_partical *pvf_pj = particals->array[j];
            wt_partical *pj = pvf_pj->partical;
            wt_vec pij = wt_vsub(pi->pos, pj->pos);
            wt_r32 len2 = wt_vlen2(pij);
            if (len2 != 0 && len2 < h * h)
            {
                //wt_debug("near viscosity_update_vel \n", 1);
                wt_r32 len = wt_sqrt(len2);
                wt_vec pij_normal = wt_vunit(pij);

                wt_vec dv = wt_vsub(pi->vel, pj->vel); // 注意 很关键减的顺序
                wt_r32 vn = wt_vdot(dv, pij_normal);
                if (vn > 0.0)
                {
                    wt_r32 q = len / h;

                    wt_r32 I = dt * (1 - q) * (f->sigma * vn + f->beta * vn * vn);
                    wt_vec tmp = wt_vmuls(pij_normal, I * 0.5);
                    pi->vel = wt_vsub(pi->vel, tmp);
                    pj->vel = wt_vadd(pj->vel, tmp);
                }
            }
        }
    }
}

void wt_pvf_partical_update(wt_pvf_fluid *f, wt_r32 dt)
{
    wt_array *particals = f->pvf_particals;
    for (int i = 0 ; i < particals->num ; i++)
    {
        wt_pvf_partical *pvf_pi = particals->array[i];
        wt_partical *pi = pvf_pi->partical;
        wt_partical_update(pi, dt);
    }

}


void wt_pvf_partical_collide_border(wt_partical *pi)
{
    if (pi->pos.x < 5 )
    {
        pi->pos.x = 5;
        pi->vel.x = - pi->vel.x;
    }

    if (pi->pos.x > 95 )
    {
        pi->pos.x = 95;
        pi->vel.x = - pi->vel.x;
    }

    if (pi->pos.y < 5)
    {
        pi->pos.y = 5;
        pi->vel.y = - pi->vel.y;
    }

    if (pi->pos.y > 95)
    {
        pi->pos.y = 95;
        pi->vel.y = - pi->vel.y;
    }
}

void wt_pvf_restrict_vel(wt_partical *pi, wt_r32 max_vel)
{
    pi->vel.x = (pi->vel.x > max_vel) ? max_vel : pi->vel.x;
    pi->vel.y = (pi->vel.y > max_vel) ? max_vel : pi->vel.y;
    pi->vel.x = (pi->vel.x < -max_vel) ? -max_vel : pi->vel.x;
    pi->vel.y = (pi->vel.y < -max_vel) ? -max_vel : pi->vel.y;
}

void wt_pvf_partical_reupdate(wt_pvf_fluid *f, wt_r32 dt)
{
    wt_array *particals = f->pvf_particals;
    for (int i = 0 ; i < particals->num ; i++)
    {
        wt_pvf_partical *pvf_pi = particals->array[i];
        wt_partical *pi = pvf_pi->partical;
        pi->vel = wt_vmuls(wt_vsub(pi->pos, pi->pre_pos), 1.0 / dt);

        //gravity
        pi->vel = wt_vadd(pi->vel, wt_vmuls(wt_v(0, -10.0), dt));

        wt_pvf_partical_collide_border(pi);
        wt_pvf_restrict_vel(pi, f->partical_max_vel);
    }
}

void wt_double_density_relax(wt_pvf_fluid *f, wt_r32 dt)
{
    wt_array *particals = f->pvf_particals;
    wt_r32 h = f->h;
    wt_r32 k = f->k;
    wt_r32 k_near = f->k_near;
    for (int i = 0 ; i < particals->num ; i++)
    {
        wt_pvf_partical *pvf_pi = particals->array[i];
        wt_partical *pi = pvf_pi->partical;
        for (int j = i + 1; j < particals->num ; j++)
        {
            wt_pvf_partical *pvf_pj = particals->array[j];
            wt_partical *pj = pvf_pj->partical;
            wt_vec pij = wt_vsub(pi->pos, pj->pos);
            wt_r32 len2 = wt_vlen2(pij);
            if (len2 != 0 && len2 < h * h)
            {
                //wt_debug("near viscosity_update_vel \n", 1);
                wt_r32 len = wt_sqrt(len2);
                //wt_vec pij_normal = wt_vunit(pij);
                wt_r32 q = len / h;
                pvf_pi->p_density = pvf_pi->p_density + (1-q) * (1-q);
                pvf_pi->p_density_near = pvf_pi->p_density_near + (1-q) * (1-q) *(1-q);

            }
        }
        pvf_pi->p_press = k * (pvf_pi->p_density - f->density);
        pvf_pi->p_press_near = k_near * pvf_pi->p_density_near;
        wt_vec dx = wt_v(0,0);
        for(int j = i + 1; j < particals->num ; j++) {
            wt_pvf_partical *pvf_pj = particals->array[j];
            wt_partical *pj = pvf_pj->partical;
            wt_vec pij = wt_vsub(pj->pos, pi->pos); //注意减的顺序
            wt_r32 len2 = wt_vlen2(pij);
            if (len2 != 0 && len2 < h * h)
            {
                wt_r32 len = wt_sqrt(len2);
                wt_r32 q = len / h;
                wt_r32 D = dt*dt;
                wt_vec pij_normal = wt_vunit(pij);
                D *= (pvf_pi->p_press * (1-q)+pvf_pi->p_press_near * (1-q) * (1-q));
                pj->pos = wt_vadd(pj->pos,wt_vmuls(pij_normal,D*0.5));
                dx = wt_vsub(dx,wt_vmuls(pij_normal,D*0.5));
            }
        }
        pi->pos = wt_vadd(pi->pos, dx);
    }

}

void wt_pvf_update_fluid(wt_pvf_fluid *f, wt_r32 dt)
{
    wt_pvf_viscosity_update_vel(f, dt);
    wt_pvf_partical_update(f, dt);

    wt_double_density_relax(f, dt);
    wt_pvf_partical_reupdate(f, dt);
}
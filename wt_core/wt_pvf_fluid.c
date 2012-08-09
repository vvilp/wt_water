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
    f->density = 20.0;
    f->h = 3 ;

    f->sigma = 3;
    f->beta = 3;
    f->k =0.5  ; //和温度有关，代表温度
    f->k_near = 0.5;//和温度有关，代表温度
    f->pvf_particals = wt_array_init(100);
    f->pvf_particals_table = wt_create_spatial_table(100.0, 1.0);
    f->partical_max_vel = 100.0;
    return f;
}
void wt_pvf_add_partical(wt_pvf_fluid *f, wt_pvf_partical *p)
{
    wt_array_add(f->pvf_particals, p);
    //wt_array_add(f->pvf_particals_table->);
    wt_spatial_table_add_obj(f->pvf_particals_table,p,p->partical->pos.x,p->partical->pos.y,f->h);
}

void wt_partical_table_reset(wt_pvf_fluid *f)
{
    for(int i = 0 ; i < f->pvf_particals_table->cell_num ; i++)
    {
        for(int j = 0 ; j < f->pvf_particals_table->cell_num ; j++){
            wt_array_clear(f->pvf_particals_table->table[i][j]);
        }
    }
    wt_array *list = f->pvf_particals_table->all_list;
    for(int i = 0 ; i < list->num ; i++){
        wt_pvf_partical *pvf_pi = list->array[i];
        //if()
        wt_spatial_table_add_obj_table_only(f->pvf_particals_table, pvf_pi, pvf_pi->partical->pos.x, pvf_pi->partical->pos.y, f->h);
    }
}

void wt_partical_table_update(wt_spatial_table *table, wt_pvf_partical *pvf_p,wt_vec pos,wt_vec pre_pos,wt_r32 r)
{

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
                wt_vec pij_normal = wt_vmuls(pij,1.0/len);

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

//使用粘度来更新速度
void wt_pvf_viscosity_update_vel_table_version(wt_pvf_fluid *f, wt_r32 dt)
{
    wt_array *particals = f->pvf_particals_table->all_list;
    wt_r32 h = f->h;
    for (int i = 0 ; i < particals->num ; i++)
    {
        wt_pvf_partical *pvf_pi = particals->array[i];
        wt_partical *pi = pvf_pi->partical;
        wt_spatial_table_get_near_list(f->pvf_particals_table, pvf_pi, pvf_pi->partical->pos.x, pvf_pi->partical->pos.y, f->h);
        wt_array *near_list = f->pvf_particals_table->near_list;
        //wt_debug("near_list_num %d \n", near_list->num);
        wt_i32 num = near_list->num > 20 ? 20 : near_list->num;
        for (int j = 0; j < num ; j++)
        {
            wt_pvf_partical *pvf_pj = near_list->array[j];
            wt_partical *pj = pvf_pj->partical;
            wt_vec pij = wt_vsub(pi->pos, pj->pos);
            wt_r32 len2 = wt_vlen2(pij);
            if (len2 != 0 && len2 < h * h)
            {
                //wt_debug("near viscosity_update_vel \n", 1);
                wt_r32 len = wt_sqrt(len2);
                wt_vec pij_normal = wt_vmuls(pij,1.0/len);

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
        wt_partical_restrict_vel(pi, f->partical_max_vel);
    }
}

//粒子位置更新，记录当前位置
void wt_pvf_partical_update(wt_pvf_fluid *f, wt_r32 dt)
{
    wt_array *particals = f->pvf_particals;
    for (int i = 0 ; i < particals->num ; i++)
    {
        wt_pvf_partical *pvf_pi = particals->array[i];
        wt_partical *pi = pvf_pi->partical;
        wt_partical_update(pi, dt);
        wt_partical_collide_border(pi);
    }
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

        wt_partical_collide_border(pi);
        wt_partical_restrict_vel(pi, f->partical_max_vel);
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
                wt_vec pij_normal = wt_vmuls(pij,1.0/len);
                D *= (pvf_pi->p_press * (1-q)+pvf_pi->p_press_near * (1-q) * (1-q));
                pj->pos = wt_vadd(pj->pos,wt_vmuls(pij_normal,D*0.5));
                dx = wt_vsub(dx,wt_vmuls(pij_normal,D*0.5));
            }
        }
        pi->pos = wt_vadd(pi->pos, dx);
    }

}
void wt_double_density_relax_table_version(wt_pvf_fluid *f, wt_r32 dt)
{
    wt_array *particals = f->pvf_particals_table->all_list;
    wt_r32 h = f->h;
    wt_r32 k = f->k;
    wt_r32 k_near = f->k_near;
    for (int i = 0 ; i < particals->num ; i++)
    {
        wt_pvf_partical *pvf_pi = particals->array[i];
        wt_partical *pi = pvf_pi->partical;
        wt_spatial_table_get_near_list(f->pvf_particals_table, pvf_pi, pvf_pi->partical->pos.x, pvf_pi->partical->pos.y, f->h);
        wt_array *near_list = f->pvf_particals_table->near_list;
        wt_i32 num = near_list->num > 20 ? 20 :  near_list->num;
        for (int j = 0; j < near_list->num ; j++)
        {
            wt_pvf_partical *pvf_pj = near_list->array[j];
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
        //wt_spatial_table_get_near_list(f->pvf_particals_table, pvf_pi, pvf_pi->partical->pos.x, pvf_pi->partical->pos.y, f->h);
        //wt_array *near_list = f->pvf_particals_table->near_list;
        for(int j = 0; j < num ; j++) {
            wt_pvf_partical *pvf_pj = near_list->array[j];
            wt_partical *pj = pvf_pj->partical;
            wt_vec pij = wt_vsub(pj->pos, pi->pos); //注意减的顺序
            wt_r32 len2 = wt_vlen2(pij);
            if (len2 != 0 && len2 < h * h)
            {
                wt_r32 len = wt_sqrt(len2);
                wt_r32 q = len / h;
                wt_r32 D = dt*dt;
                wt_vec pij_normal = wt_vmuls(pij,1.0/len);
                D *= (pvf_pi->p_press * (1-q)+pvf_pi->p_press_near * (1-q) * (1-q));
                //pj->pre_pos = pj->pos;
                pj->pos = wt_vadd(pj->pos,wt_vmuls(pij_normal,D*0.5));
                dx = wt_vsub(dx,wt_vmuls(pij_normal,D*0.5));
            }
        }
        //pi->pre_pos = pi->pos;
        pi->pos = wt_vadd(pi->pos, dx);
    }

}

void wt_pvf_update_fluid(wt_pvf_fluid *f, wt_r32 dt)
{
    
    wt_pvf_viscosity_update_vel_table_version(f, dt);
    //wt_pvf_viscosity_update_vel(f, dt);
    
    wt_pvf_partical_update(f, dt);
    wt_partical_table_reset(f);

    //wt_double_density_relax(f, dt);
    wt_double_density_relax_table_version(f, dt);
    //wt_partical_table_reset(f);

    wt_pvf_partical_reupdate(f, dt);

}
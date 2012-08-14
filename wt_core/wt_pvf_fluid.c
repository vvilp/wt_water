#include "wt_core.h"

wt_pvf_partical*    wt_create_pvf_partical(wt_body * b)
{
    wt_pvf_partical *pvf_p = (wt_pvf_partical *) malloc (sizeof(wt_pvf_partical));
    //pvf_p -> partical = p;
    pvf_p -> body = b;
    pvf_p -> p_density = 0;
    pvf_p->p_density_near = 0;
    pvf_p->p_press = 0;
    pvf_p->p_press_near = 0;

    return pvf_p;
}
wt_pvf_fluid *wt_create_pvf_fluid()
{
    wt_pvf_fluid *f = (wt_pvf_fluid *) malloc (sizeof(wt_pvf_fluid));
    f->density = 10.0;
    f->h = 2;

    f->sigma = 6;
    f->beta = 1;
    f->k = 0.05  ; //和温度有关，代表温度 0.005:类似激烈的洋流
    f->k_near = 1;//和温度有关，代表温度
    f->pvf_particals = wt_array_init(100);
    f->pvf_particals_table = wt_create_spatial_table(100.0, 1.0);
    f->partical_max_vel = 100.0;

    f->k_spring = 1000;
    f->spring_rest_len = 1.6;
    return f;
}
void wt_pvf_add_partical(wt_pvf_fluid *f, wt_pvf_partical *p)
{
    wt_array_add(f->pvf_particals, p);
    //wt_array_add(f->pvf_particals_table->);
    wt_spatial_table_add_obj(f->pvf_particals_table,p,p->body->pos.x,p->body->pos.y,f->h);
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
        wt_spatial_table_add_obj_table_only(f->pvf_particals_table, pvf_pi, pvf_pi->body->pos.x, pvf_pi->body->pos.y, f->h);
    }
}

//粒子位置更新，记录当前位置
void wt_pvf_partical_update(wt_pvf_fluid *f, wt_r32 dt)
{
    wt_array *particals = f->pvf_particals;
    for (int i = 0 ; i < particals->num ; i++)
    {
        wt_pvf_partical *pvf_pi = particals->array[i];
        //wt_partical *pi = pvf_pi->partical;
        //pi->vel = wt_vadd(pi->vel, wt_vmuls(wt_v(0, -10.0), dt));

        wt_body *bi = pvf_pi->body;
        // wt_partical_update(pi, dt);
        // wt_partical_restrict_vel(pi, f->partical_max_vel);
        // wt_partical_collide_border(pi);
        wt_body_update_step(bi, dt);
        wt_body_restrict_vel(bi,f->partical_max_vel);
        wt_body_collide_border(bi);
    }
}

//使用当前位置和之前记录的位置，重新设定速度，使速度不至于变化太大
void wt_pvf_partical_reupdate(wt_pvf_fluid *f, wt_r32 dt)
{
    wt_array *particals = f->pvf_particals;
    for (int i = 0 ; i < particals->num ; i++)
    {
        wt_pvf_partical *pvf_pi = particals->array[i];
        //wt_partical *pi = pvf_pi->partical;
        //pi->vel = wt_vmuls(wt_vsub(pi->pos, pi->pre_pos), 1.0 / dt);
        wt_body *bi = pvf_pi->body;
        bi->vel = wt_vmuls(wt_vsub(bi->pos, bi->pre_pos), 1.0 / dt);

        wt_body_restrict_vel(bi,f->partical_max_vel);
        wt_body_collide_border(bi);
    }
}

//简单循环版本
// void wt_pvf_viscosity_update_vel(wt_pvf_fluid *f, wt_r32 dt)
// {
//     wt_array *particals = f->pvf_particals;
//     wt_r32 h = f->h;
//     for (int i = 0 ; i < particals->num ; i++)
//     {
//         wt_pvf_partical *pvf_pi = particals->array[i];
//         wt_partical *pi = pvf_pi->partical;
//         for (int j = i + 1; j < particals->num ; j++)
//         {
//             wt_pvf_partical *pvf_pj = particals->array[j];
//             wt_partical *pj = pvf_pj->partical;
//             wt_vec pij = wt_vsub(pi->pos, pj->pos);
//             wt_r32 len2 = wt_vlen2(pij);
//             if (len2 != 0 && len2 < h * h)
//             {
//                 //wt_debug("near viscosity_update_vel \n", 1);
//                 wt_r32 len = wt_sqrt(len2);
//                 wt_vec pij_normal = wt_vmuls(pij,1.0/len);

//                 wt_vec dv = wt_vsub(pi->vel, pj->vel); // 注意 很关键减的顺序
//                 wt_r32 vn = wt_vdot(dv, pij_normal);
//                 if (vn > 0.0)
//                 {
//                     wt_r32 q = len / h;

//                     wt_r32 I = dt * (1 - q) * (f->sigma * vn + f->beta * vn * vn);
//                     wt_vec tmp = wt_vmuls(pij_normal, I * 0.5);
//                     pi->vel = wt_vsub(pi->vel, tmp);
//                     pj->vel = wt_vadd(pj->vel, tmp);
//                 }
//             }
//         }
//     }
// }

//使用spatial table 版本，使用粘度来更新速度
void wt_pvf_viscosity_update_vel_table_version(wt_pvf_fluid *f, wt_r32 dt)
{
    wt_array *particals = f->pvf_particals_table->all_list;
    wt_r32 h = f->h;
    for (int i = 0 ; i < particals->num ; i++)
    {
        wt_pvf_partical *pvf_pi = particals->array[i];
        wt_body *bi = pvf_pi->body;
        wt_spatial_table_get_near_list(f->pvf_particals_table, pvf_pi, bi->pos.x, bi->pos.y, f->h);
        wt_array *near_list = f->pvf_particals_table->near_list;
        //wt_debug("near_list_num %d \n", near_list->num);
        wt_i32 num = near_list->num > 20 ? 20 : near_list->num;
        for (int j = 0; j < num ; j++)
        {
            wt_pvf_partical *pvf_pj = near_list->array[j];
            wt_body *bj = pvf_pj->body;
            wt_vec pij = wt_vsub(bi->pos, bj->pos);
            wt_r32 len2 = wt_vlen2(pij);
            if (len2 != 0 && len2 < h * h)
            {
                //wt_debug("near viscosity_update_vel \n", 1);
                wt_r32 len = wt_sqrt(len2);
                wt_vec pij_normal = wt_vmuls(pij,1.0/len);

                wt_vec dv = wt_vsub(bi->vel, bj->vel); // 注意 很关键减的顺序
                wt_r32 vn = wt_vdot(dv, pij_normal);
                if (vn > 0.0)
                {
                    wt_r32 q = len / h;
                    wt_r32 I = dt * (1 - q) * (f->sigma * vn + f->beta * vn * vn);
                    wt_vec tmp = wt_vmuls(pij_normal, I * 0.5);
                    bi->vel = wt_vsub(bi->vel, tmp);
                    bj->vel = wt_vadd(bj->vel, tmp);
                }
            }
        }
        wt_body_restrict_vel(bi,f->partical_max_vel);
    }
}


//简单循环版本
// void wt_double_density_relax(wt_pvf_fluid *f, wt_r32 dt)
// {
//     wt_array *particals = f->pvf_particals;
//     wt_r32 h = f->h;
//     wt_r32 k = f->k;
//     wt_r32 k_near = f->k_near;
//     for (int i = 0 ; i < particals->num ; i++)
//     {
//         wt_pvf_partical *pvf_pi = particals->array[i];
//         wt_partical *pi = pvf_pi->partical;
//         for (int j = i + 1; j < particals->num ; j++)
//         {
//             wt_pvf_partical *pvf_pj = particals->array[j];
//             wt_partical *pj = pvf_pj->partical;
//             wt_vec pij = wt_vsub(pi->pos, pj->pos);
//             wt_r32 len2 = wt_vlen2(pij);
//             if (len2 != 0 && len2 < h * h)
//             {
//                 //wt_debug("near viscosity_update_vel \n", 1);
//                 wt_r32 len = wt_sqrt(len2);
//                 //wt_vec pij_normal = wt_vunit(pij);
//                 wt_r32 q = len / h;
//                 pvf_pi->p_density = pvf_pi->p_density + (1-q) * (1-q);
//                 pvf_pi->p_density_near = pvf_pi->p_density_near + (1-q) * (1-q) *(1-q);

//             }
//         }
//         pvf_pi->p_press = k * (pvf_pi->p_density - f->density);
//         pvf_pi->p_press_near = k_near * pvf_pi->p_density_near;
//         wt_vec dx = wt_v(0,0);
//         for(int j = i + 1; j < particals->num ; j++) {
//             wt_pvf_partical *pvf_pj = particals->array[j];
//             wt_partical *pj = pvf_pj->partical;
//             wt_vec pij = wt_vsub(pj->pos, pi->pos); //注意减的顺序
//             wt_r32 len2 = wt_vlen2(pij);
//             if (len2 != 0 && len2 < h * h)
//             {
//                 wt_r32 len = wt_sqrt(len2);
//                 wt_r32 q = len / h;
//                 wt_r32 D = dt*dt;
//                 wt_vec pij_normal = wt_vmuls(pij,1.0/len);
//                 D *= (pvf_pi->p_press * (1-q)+pvf_pi->p_press_near * (1-q) * (1-q));
//                 pj->pos = wt_vadd(pj->pos,wt_vmuls(pij_normal,D*0.5));
//                 dx = wt_vsub(dx,wt_vmuls(pij_normal,D*0.5));
//             }
//         }
//         pi->pos = wt_vadd(pi->pos, dx);
//     }

// }

//使用spatial table 版本，计算密度与压强，更新粒子位置
void wt_double_density_relax_table_version(wt_pvf_fluid *f, wt_r32 dt)
{
    wt_array *particals = f->pvf_particals_table->all_list;
    wt_r32 h = f->h;
    wt_r32 k = f->k;
    wt_r32 k_near = f->k_near;
    wt_r32 k_spring = f->k_spring;
    for (int i = 0 ; i < particals->num ; i++)
    {
        wt_pvf_partical *pvf_pi = particals->array[i];
        wt_body *bi = pvf_pi->body;
        wt_spatial_table_get_near_list(f->pvf_particals_table, pvf_pi, bi->pos.x, bi->pos.y, f->h);
        wt_array *near_list = f->pvf_particals_table->near_list;
        wt_i32 num = near_list->num > 20 ? 20 :  near_list->num;
        for (int j = 0; j < near_list->num ; j++)
        {
            wt_pvf_partical *pvf_pj = near_list->array[j];
            wt_body *bj = pvf_pj->body;
            wt_vec pij = wt_vsub(bi->pos, bj->pos);
            wt_r32 len2 = wt_vlen2(pij);
            if (len2 != 0 && len2 < h * h)
            {
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

        for(int j = 0; j < num ; j++) {
            wt_pvf_partical *pvf_pj = near_list->array[j];
            wt_body *bj = pvf_pj->body;
            wt_vec pij = wt_vsub(bj->pos, bi->pos); //注意减的顺序
            wt_r32 len2 = wt_vlen2(pij);
            if (len2 != 0 && len2 < h * h)
            {
                wt_r32 len = wt_sqrt(len2);
                wt_r32 q = len / h;
                wt_r32 D = dt*dt;
                wt_vec pij_normal = wt_vmuls(pij,1.0/len);

                //加入弹簧，用于调整流体塑形
                wt_r32 d_spring = dt * dt * k_spring * (1 - f->spring_rest_len / h) * (f->spring_rest_len - len);
                wt_vec D_spring = wt_vmuls(pij_normal,d_spring);
                bi->pos = wt_vsub(bi->pos,D_spring);
                bj->pos = wt_vadd(bj->pos,D_spring);

                D *= (pvf_pi->p_press * (1-q)+pvf_pi->p_press_near * (1-q) * (1-q));
                bj->pos = wt_vadd(bj->pos,wt_vmuls(pij_normal,D*0.5));
                dx = wt_vsub(dx,wt_vmuls(pij_normal,D*0.5));

            }
        }
        bi->pos = wt_vadd(bi->pos, dx);
    }
}


//测试函数，流体弹性,使用弹簧模拟
// void wt_pvf_elasticity_update(wt_pvf_fluid *f, wt_r32 dt)
// {
//     wt_array *particals = f->pvf_particals_table->all_list;
//     wt_r32 h = f->h;
//     wt_r32 k_spring = f->k_spring;
//     for (int i = 0 ; i < particals->num ; i++)
//     {
//         wt_pvf_partical *pvf_pi = particals->array[i];
//         wt_partical *pi = pvf_pi->partical;
//         wt_spatial_table_get_near_list(f->pvf_particals_table, pvf_pi, pvf_pi->partical->pos.x, pvf_pi->partical->pos.y, f->h);
//         wt_array *near_list = f->pvf_particals_table->near_list;
//         wt_i32 num = near_list->num > 20 ? 20 :  near_list->num;
//         for (int j = 0; j < near_list->num ; j++)
//         {
//             wt_pvf_partical *pvf_pj = near_list->array[j];
//             wt_partical *pj = pvf_pj->partical;
//             wt_vec pji = wt_vsub(pj->pos, pi->pos);
//             wt_r32 len2 = wt_vlen2(pji);
//             if (len2 != 0 && len2 < h * h)
//             {
//                 wt_r32 d = dt * dt * k_spring * (1 - f->spring_rest_len / h) * (f->spring_rest_len - sqrt(len2));
//                 wt_vec D = wt_vmuls(wt_vunit(pji),d);
//                 pi->pos = wt_vsub(pi->pos,D);
//                 pj->pos = wt_vadd(pj->pos,D);
//             }

//         }
//     }
// }

void wt_pvf_update_fluid(wt_pvf_fluid *f, wt_r32 dt)
{
    wt_pvf_viscosity_update_vel_table_version(f, dt); 
    
    wt_pvf_partical_update(f, dt);

    wt_partical_table_reset(f);

    wt_double_density_relax_table_version(f, dt);

    wt_pvf_partical_reupdate(f, dt);
}

//用于外力牵引流体粒子
void wt_pvf_add_extern_force(wt_array *pvf_particals,wt_r32 ael, wt_vec to_pos)
{
    for(int i = 0 ; i < pvf_particals->num ; i++) {
        wt_pvf_partical * pvf_p = pvf_particals->array[i];
        wt_body *b = pvf_p->body;
        // wt_vec normal = wt_vunit(wt_vsub(to_pos,p->pos));
        // p->ael = wt_vadd(wt_v(0,-10.0),wt_vmuls(normal,ael));
        wt_vec normal = wt_vmuls(wt_vsub(to_pos,b->pos),ael);
        b->ael = wt_vadd(wt_v(0,-10.0),normal);
    }
}

//选择一定范围的粒子
void wt_pvf_choose_range_particals(wt_array *all_pvf_particals,wt_vec pos, wt_r32 range,wt_array *choose_particals)
{
    wt_array_clear(choose_particals);
    for(int i = 0 ; i < all_pvf_particals->num ; i++){
        wt_pvf_partical * pvf_p = all_pvf_particals->array[i];
        wt_body *b = pvf_p->body;
        wt_r32 len2 = wt_vlen2(wt_vsub(b->pos, pos));
        if(len2 < range * range){
            wt_array_add(choose_particals,pvf_p);
        }
    }
}

//设置粒子加速度
void wt_pvf_set_partical_ael(wt_array *pvf_particals, wt_vec ael)
{
    for(int i = 0 ; i < pvf_particals->num ; i++){
        wt_pvf_partical * pvf_p = pvf_particals->array[i];
        wt_body *b = pvf_p->body;
        b->ael = ael;
    }
}
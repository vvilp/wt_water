#include "wt_core.h"

void wt_collision_clear_contacts(wt_world *w)
{
    wt_array *contacts = w->contacts;
    for (int i = 0 ; i < contacts->num ; i++)
    {
        wt_contact *c = contacts->array[i];
        free(c);
    }
    wt_array_clear(contacts);
}

void wt_collision_detect(wt_world *w)
{
    wt_collision_clear_contacts(w);
    wt_array *shapes = w->shapes;
    for (int i = 0; i < shapes->num; ++i)
    {
        wt_shape *s1 = shapes->array[i];
        for (int j = i + 1; j < shapes->num; ++j)
        {
            wt_shape *s2 = shapes->array[j];
            wt_shape2shape_collision(s1, s2, w->contacts);
        }
    }
}

void wt_contact_solve(wt_contact *contact, wt_r32 dt)
{
    wt_body *b1 = contact->b1;
    wt_body *b2 = contact->b2;

    wt_vec r1 = wt_vsub(contact->pos, contact->b1->pos);
    wt_vec r2 = wt_vsub(contact->pos, contact->b2->pos);

    wt_vec dv = wt_vadd(b2->vel , wt_ScrossV(b2->ang_vel, r2));
    dv = wt_vsub(dv, wt_vadd(b1->vel , wt_ScrossV(b1->ang_vel, r1)));

    wt_r32 vn = wt_vdot(dv, contact->normal);
    wt_r32 dPn = (-(1 + contact->restitution) * vn + contact->bias) / contact->k_normal;

    //wt_debug("vn:%f\n",vn);
    //wt_debug("contact->bias:%f\n",contact->bias);
    //wt_debug("contact->restitution:%f\n",contact->restitution);
    //wt_debug("dPn1:%f\n",dPn);
    //accumulate impulse
    wt_r32 temp = contact->impulse_n;
    //wt_debug("contact->impulse_n1:%f\n",contact->impulse_n);
    contact->impulse_n = wt_rmax(temp + dPn, 0.0f);
    //wt_debug("contact->impulse_n1:%f\n",contact->impulse_n);
    dPn = contact->impulse_n - temp;
    //wt_debug("dPn2:%f\n",dPn);
    wt_vec vpn = wt_vmuls(contact->normal, dPn); //dPn * contact->normal;

    b1->vel = wt_vsub(b1->vel, wt_vmuls(vpn, b1->inv_mas));
    b1->ang_vel -= wt_vmulv(r1, vpn) / b1-> I;
    b2->vel = wt_vadd(b2->vel, wt_vmuls(vpn, b2->inv_mas));
    b2->ang_vel += wt_vmulv(r2, vpn) / b2-> I;

    //wt_debug("contact->k_normal:%f\n",contact->k_normal);
    //wt_debug("vn:%f\n", vn);
    //wt_debug("contact->normal  x:%f, y:%f\n",contact->normal.x, contact->normal.y);
    //wt_debug("dv1  x:%f, y:%f\n",dv.x, dv.y);
    //wt_debug("vpn  x:%f, y:%f\n",vpn.x, vpn.y);
    //wt_debug("contact->normal  x:%f, y:%f\n",contact->normal.x, contact->normal.y);
    //wt_debug("b1->vel  x:%f, y:%f\n",b1->vel.x, b1->vel.y);
    //wt_debug("b2->vel  x:%f, y:%f\n",b2->vel.x, b2->vel.y);
    //--------------------------------------------------------
    //计算摩擦冲量和速度
    //用来计算的角度必须使用弧度制。
    wt_r32 friction = ( b1->fric + b2->fric ) / 2;
    dv = wt_vadd(b2->vel , wt_ScrossV(b2->ang_vel, r2));
    dv = wt_vsub(dv, wt_vadd(b1->vel , wt_ScrossV(b1->ang_vel, r1)));


    //wt_debug("dv2  x:%f, y:%f\n",dv.x, dv.y);
    wt_vec tangent = wt_vnperp(contact->normal);
    //wt_debug("tangent  x:%f, y:%f\n",tangent.x,tangent.y);

    wt_r32 vt = wt_vdot(dv, tangent);
    wt_r32 dPt = (-vt) / contact->k_tangent ;

    //accumulate impulse
    wt_r32 maxPt = friction * contact->impulse_n;
    temp = contact->impulse_t;
    contact->impulse_t = wt_rclamp(temp + dPt, -maxPt, maxPt);
    dPt = contact->impulse_t - temp;

    wt_vec vpt = wt_vmuls(tangent, dPt);
    //wt_debug("vpt  x:%f, y:%f\n",vpt.x,vpt.y);
    b1->vel = wt_vsub(b1->vel, wt_vmuls(vpt, b1->inv_mas));
    b1->ang_vel -= wt_vmulv(r1, vpt) / b1-> I;
    b2->vel = wt_vadd(b2->vel, wt_vmuls(vpt, b2->inv_mas));
    b2->ang_vel += wt_vmulv(r2, vpt) / b2-> I;

    //wt_debug("b1->ang_vel:%f \n",b1->ang_vel);
    //wt_debug("b2->ang_vel:%f \n",b2->ang_vel);

}


void wt_contact_before_solve(wt_contact *contact, wt_r32 dt)
{
    //c->bias = -k_biasFactor * inv_dt * Min(0.0f, c->separation + k_allowedPenetration);
    contact->bias = -0.3 * 1 / dt * wt_rmin(0.0f, contact->separation + contact->allow_penetration);
    //wt_debug("before_solve contact->bias:%f\n",contact->bias);
    wt_vec r1 = wt_vsub(contact->pos, contact->b1->pos);
    wt_vec r2 = wt_vsub(contact->pos, contact->b2->pos);

    wt_r32 rn1 = wt_vdot(r1, contact->pos);
    wt_r32 rn2 = wt_vdot(r2, contact->pos);
    contact->k_normal = contact->b1->inv_mas + contact->b2->inv_mas;
    contact->k_normal += (wt_vdot(r1, r1) - rn1 * rn1) / contact->b1->I;
    contact->k_normal += (wt_vdot(r2, r2) - rn2 * rn2) / contact->b2->I;

    wt_vec tangent = wt_vnperp(contact->normal);
    wt_r32 rt1 = wt_vdot(r1, tangent);
    wt_r32 rt2 = wt_vdot(r2, tangent);
    contact->k_tangent = contact->b1->inv_mas + contact->b2->inv_mas;
    contact->k_tangent += (wt_vdot(r1, r1) - rt1 * rt1) / contact->b1->I;
    contact->k_tangent += (wt_vdot(r2, r2) - rt2 * rt2) / contact->b2->I;

    //wt_debug("(wt_vdot(r1, r1) - rn1 * rn1) / contact->b1->I: %f\n", (wt_vdot(r1, r1) - rn1 * rn1) / contact->b1->I);
    //wt_debug("contact->bias: %d\n", contact->bias);
    //wt_debug("before_solve contact->k_normal: %f\n", contact->k_normal);
    //wt_debug("before_solve contact->k_tangent: %f\n", contact->k_tangent);


    //另外两种计算方式 用来比较
    wt_r32 temp_k_normal = contact->b1->inv_mas + contact->b2->inv_mas;
    wt_vec t1 = wt_vdiv(wt_ScrossV(wt_vmulv(r1, contact->normal), r1), contact->b1->I);
    wt_vec t2 = wt_vdiv(wt_ScrossV(wt_vmulv(r2, contact->normal), r2), contact->b2->I);
    temp_k_normal += wt_vdot(wt_vadd(t1, t2), contact->normal);
    //wt_debug("before_solve temp_k_normal: %f\n", temp_k_normal);
    //---------------------------------------------------------------------------------

    wt_r32 temp_k_normal2 = contact->b1->inv_mas + contact->b2->inv_mas;
    wt_r32 t3 = wt_vmulv(r1, contact->normal);
    t3 = t3 * t3 / contact->b1->I;
    wt_r32 t4 = wt_vmulv(r2, contact->normal);
    t4 = t4 * t4 / contact->b2->I;
    temp_k_normal2 += t3 + t4;
    //wt_debug("before_solve temp_k_normal2: %f\n", temp_k_normal2);

    wt_r32 temp_k_tangent2 = contact->b1->inv_mas + contact->b2->inv_mas;
    t3 = wt_vmulv(r1, tangent);
    t3 = t3 * t3 / contact->b1->I;
    t4 = wt_vmulv(r2, tangent);
    t4 = t4 * t4 / contact->b2->I;
    temp_k_tangent2 += t3 + t4;
    //wt_debug("before_solve temp_k_normal2: %f\n", temp_k_tangent2);


    contact->k_normal = temp_k_normal2;
    contact->k_tangent = temp_k_tangent2;

}


//更新碰撞之前的准备工作，计算bias k_normal k_tangent
void wt_collision_before_solve(wt_world *w, wt_r32 dt)
{
    wt_array *contacts = w->contacts;

    for (int i = 0; i < contacts->num; ++i)
    {
        wt_contact *contact = (wt_contact *)contacts->array[i];
        wt_contact_before_solve(contact, dt);
    }

}

void wt_collision_solve(wt_world *w, wt_r32 dt)
{
    wt_array *contacts = w->contacts;
    for (int j = 0; j < 3; j++)
    {
        for (int i = 0; i < contacts->num; ++i)
        {
            wt_contact *contact = (wt_contact *)contacts->array[i];
            wt_contact_solve(contact, dt);
        }
    }
}


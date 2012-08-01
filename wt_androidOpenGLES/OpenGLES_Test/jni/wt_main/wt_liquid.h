#ifndef WT_LIQUID_H
#define WT_LIQUID_H

#include "wt_all.h"
#include "../wt_common/wt_common.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
static wt_spring* wt_create_spring(wt_body* b1,wt_body* b2 ,wt_r32 maxlen,wt_r32 restlen,wt_r32 coef,wt_r32 damp)
{
	wt_spring *spring = (wt_spring *) malloc (sizeof(wt_spring));
	spring->b1       = b1;
	spring->b2       = b2;
	spring->maxlen   = maxlen;
	spring->relaxlen = restlen;
	spring->coef     = coef;
	spring->damp     = damp;
	return spring;
}


//随机加入body
static wt_status wt_liquid_create_body(wt_liquid *l,wt_i32 num){
	wt_array * bodys = l->bodys;
	srand((unsigned) time(NULL));
	for(int i = 0 ; i < num ; i++){
		wt_vec t_pos = wt_v(l->pos.x + (rand() % 20 - rand() % 20),l->pos.y + (rand() % 20 - rand() % 20));
		wt_body* b = wt_create_body0(10,t_pos,0);
		// 在20的范围内 随机摆放body
		wt_array_add(bodys,b);
	}
}


static wt_status wt_liquid_add_spring(wt_liquid *l,wt_spring *s){
	wt_array *springs = l->springs;

	//没建立关系 再加入
	for(int i = 0 ; i < springs->num ; i++){
		wt_spring *spr = springs->array[i];
		if(spr->b1 == s->b1 && spr->b2 == s->b2) return 0;
	}

	wt_array_add(l->springs,s);
}

/**
 * 每个分子与周围最近的num个球建立力作用
 */
static wt_status wt_liquid_create_force(wt_liquid *l,wt_i32 num){
	wt_array *bodys = l->bodys;

	wt_r32 mindis[100];
	for(int i = 0 ; i < num ; i++){
		mindis[i] = 99999;
	}
	//printf("mindis[0]:%f,mindis[99]:%f",mindis[0],mindis[99]);
	wt_body *mindis_body[100];
	for(int i = 0 ; i < bodys->num ; i++){
		wt_body *ib = bodys->array[i];
		for(int j = 0 ; j < bodys->num ; j++){
			if(i!=j){
				wt_body *jb = bodys->array[j];
				wt_r32 dis = wt_vlen2(wt_vsub(ib->pos,jb->pos));

				for(int k = 0 ; k < num ; k++){
					if(dis < mindis[k]){
						for(int t = num-1 ; t >=k+1 ; t--){
							mindis[t] = mindis[t-1];
							mindis_body[t] = mindis_body[t-1];
						}
						mindis[k] = dis;
						mindis_body[k] = jb;
					}
				}
			}
		}
		//将与最近的num个物体建立的关系加入
		for(int k = 0 ; k < num ; k++){
			wt_spring *spr = wt_create_spring(ib,mindis_body[k],5,10,10,0.8);
			wt_liquid_add_spring(l,spr);
		}
	}
}
static wt_liquid * wt_create_liquid() {
	wt_liquid * l = (wt_liquid *) malloc (sizeof(wt_liquid));
	l->bodys      = wt_array_init(10);
	l->springs    = wt_array_init(10);
	l->force_num  = 3;
	l->pos        = wt_v(50, 50);
	//生成body
	wt_liquid_create_body(l,10);

	//建立作用力
	wt_liquid_create_force(l,l->force_num);
	return l;
}





#endif

#ifndef _WT_CIR_H_
#define _WT_CIR_H_

#include "wt_core.h"

typedef struct {
	wt_body *body;
	wt_r32 radius;
	//圆形转动惯量  MR^2/2
} wt_circle;

static wt_circle *wt_create_cir(wt_body *body, wt_r32 radius) {
	wt_circle *c = NULL;
	c = (wt_circle *) malloc(sizeof(wt_circle));
	c->body   = body;
	c->radius = radius;
	body->I   = (body->mas == WT_MAX_R32) ? WT_MAX_R32 : body->mas * radius * radius / 2;
	return c;
}

#endif
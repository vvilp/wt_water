#include "wt_core.h"


wt_circle *wt_create_cir(wt_body *body, wt_r32 radius)
{
    wt_circle *c = NULL;
    c = (wt_circle *) malloc(sizeof(wt_circle));
    c->body   = body;
    c->radius = radius;
    body->I   = (body->mas == WT_MAX_R32) ? WT_MAX_R32 : body->mas * radius * radius / 2;
    return c;
}


wt_shape *wt_create_shape(void *shape, wt_i32 type)
{
    wt_shape *s = NULL;
    s = (wt_shape *) malloc(sizeof(wt_shape));
    s->shape = shape;
    s->type = type;
    return s;
}

wt_body *wt_shape_get_body(wt_shape *shape) {
	wt_body* b;
	switch (shape->type) {
    	case WT_CIR:
    		b = ((wt_circle*) (shape->shape))->body;
    		break;
    	//case WT_REC:
    	//	b = ((wt_rect*) (shape->shape))->body;
    	//	break;
	}
	return b;
}
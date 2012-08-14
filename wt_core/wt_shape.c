#include "wt_core.h"


wt_circle *wt_create_cir(wt_body *body, wt_r32 radius)
{
    wt_circle *c = NULL;
    c = (wt_circle *) malloc(sizeof(wt_circle));
    c->body   = body;
    c->radius = radius;
    body->I   = (body->mas == WT_MAX_R32) ? WT_MAX_R32 : body->mas * radius * radius / 2;

    body->inv_I = (body->I == WT_MAX_R32) ?  0 : ((body->I == 0) ? WT_MAX_R32 : 1 / body->I);
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

wt_AABB wt_shape_get_AABB(wt_shape *shape){
    wt_body* b;
    wt_AABB aabb;
    switch (shape->type) {
        case WT_CIR:
            b = ((wt_circle*) (shape->shape))->body;
            wt_circle *c = (wt_circle*)shape->shape;
            aabb.pos_tl = wt_v(b->pos.x - c->radius , b->pos.y + c->radius);
            aabb.pos_br = wt_v(b->pos.x + c->radius , b->pos.y - c->radius);
            break;
        //case WT_REC:
        //  b = ((wt_rect*) (shape->shape))->body;
        //  break;
    }
    return aabb;
}
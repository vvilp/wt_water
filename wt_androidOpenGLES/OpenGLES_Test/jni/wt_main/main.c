#include "GL/glut.h"
#include <math.h>
#include "../wt_common/wt_common.h"
#include "wt_all.h"
#include "float.h"
#include "wt_collision2.h"

wt_world *w;

void wt_draw_collision(wt_array *contacts){
	for (int i = 0; i < contacts->num; ++i)
	{
		wt_contact* contact = contacts->array[i];
		wt_draw_dot2f(contact->contact_p.x, contact->contact_p.y);
		debug_vec(contact->contact_p, "cls p:");
	}

	wt_clear_array_contact(contacts);
}

void wt_draw_shapes(wt_array *shapes){
	wt_gl_color c;
	c.r = 0.06;c.g = 0.9;c.b = 0.221;

	wt_circle *cir;
	for (int i = 0; i < shapes->num; ++i) {
		wt_shape *s = shapes->array[i];
		if (s->type == WT_CIR) {
			cir = (wt_circle *) (s->shape);
			wt_draw_cir(*cir, c);
		}
	}
	for (int i = 0; i < shapes->num; ++i) {
		wt_shape *s = shapes->array[i];
		if (s->type == WT_REC) {
			wt_rect *r = (wt_rect *) (s->shape);
			wt_draw_rect(*r, c);
		}
	}

}

void wt_draw_liquid(wt_array *liquid){
	for (int i = 0; i < liquid->num; i++)
	{
		wt_liquid * l = liquid ->array[i];
		wt_array *bodys = l->bodys;
		printf("画图中liquid body:%d \n",bodys->num);
		for (int j = 0; j < bodys->num; j++)
		{
			wt_body *b = bodys->array[j];
			wt_draw_dot2f(b->pos.x, b->pos.y);
		}
	}
}


void wt_draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //可以正常使用shape函数
	glLoadIdentity(); //初始化原点坐标
	wt_array *shapes = w->shapes;
	//wt_array *cls = w->collision;
	wt_draw_collision(w->contact);
	wt_draw_shapes(shapes);
	wt_draw_liquid(w->liquid);
	glutSwapBuffers();
}

//临时
wt_status wt_update_collide_border(wt_world *w) {
	wt_array *ss = w->shapes;
	for (int i = 0; i < ss->num; ++i) {
		wt_shape *s = ss->array[i];
		wt_circle *c = s->shape;
		wt_body *b = c->body;
		if (b->pos.y >= 100)
			b->vel.y = -b->vel.y;
		if (b->pos.y <= -100)
			b->vel.y = -b->vel.y;
		if (b->pos.x >= 100)
			b->vel.x = -b->vel.x;
		if (b->pos.x <= -100)
			b->vel.x = -b->vel.x;
	}
}
void run() {

	wt_world_update_step(w, 1.0 / 60.0);
	wt_collision_update(w);
	wt_update_collide_border(w);

	wt_draw();
	_sleep(10);
}

void wt_spring_test() {
	wt_body *b1, *b2, *b3;
	wt_circle *c;
	wt_shape *s;
	wt_rect *r;

	b1 = wt_create_body0(1,wt_v(0,0),0);
	c = wt_create_cir(b1, 5);
	s = wt_create_shape(c, WT_CIR);
	wt_world_add_shape(w, s);

	b2 = wt_create_body0(5,wt_v(-15,0),0);
	c = wt_create_cir(b2, 5);
	s = wt_create_shape(c, WT_CIR);
	wt_world_add_shape(w, s);

	wt_spring *spr = wt_create_spring(b1, b2, 40, 30, 30,2);
	wt_world_add_spring(w, spr);

	b3 = wt_create_body0(5,wt_v(15,0),0);
	c = wt_create_cir(b3, 5);
	s = wt_create_shape(c, WT_CIR);
	wt_world_add_shape(w, s);

	spr = wt_create_spring(b1, b3, 40, 30, 30,2);
	wt_world_add_spring(w, spr);

}

void wt_wall_test(){
	wt_rect *r;
	wt_shape *s;
	wt_body * b = wt_create_body0(10,wt_v(-20,0),0);
	r = wt_create_rect(b, 2, 50);
	s = wt_create_shape(r, WT_REC);
	wt_world_add_shape(w, s);

	wt_circle *c;
	b = wt_create_body0(5,wt_v(0,100),0);
	c = wt_create_cir(b, 5);
	s = wt_create_shape(c, WT_CIR);
	wt_world_add_shape(w, s);
}

void wt_generate_body(wt_world *w) {
//	wt_body *b1,*b2;
//	wt_circle *c;
//	wt_shape *s;
//	wt_rect *r;

//	srand((unsigned) time(NULL));
//	 for (int i = 0 ; i < 30; i++)
//	 {
//	     b = wt_create_body();
//	     b->mas = 5;
//	     b->pos.x = (float)((rand()) % 100 - (rand()) % 100);
//	     b->pos.y = (float)((rand()) % 100 - (rand()) % 100);
//	     b->vel.x = (float)((rand()) % 20 - (rand()) % 20);
//	     b->vel.y = (float)((rand()) % 20 - (rand()) % 20);
//	     b->ang_vel = 1;//b->u=0.5;
//	      c = wt_create_cir(b, 5);
//	      s = wt_create_shape(c, WT_CIR);
//			//r = wt_create_rect(b, 5, 5);
//			//s = wt_create_shape(r, WT_REC);
//	     wt_world_add_shape(w, s);
//	 }

//
//		b = wt_create_body();
//		b->mas = 10;
//		b->pos.y = -0.01;
//		b->pos.x = -0.9;
//		b->vel.x = 0.5;
//		b->ang_vel = -10;
//		b->u=0.5;
//		c = wt_create_cir(b, 0.05);
//		s = wt_create_shape(c, WT_CIR);
//		wt_world_add_shape(w, s);

//		b = wt_create_body();
//		b->mas = 10;
//		b->pos.y = 0;
//		b->pos.x = 0;
//		b->u=0.5;
//		c = wt_create_cir(b, 0.05);
//		s = wt_create_shape(c, WT_CIR);
//		wt_world_add_shape(w, s);

//		b = wt_create_body();
//		b->mas = 10;
//		//b->vel = wt_v(5,0);
//		b->u=0.5;
//		//b->angular = 100.0f / 180.0f * WT_PI;
//		r = wt_create_rect(b, 5, 5);
//		s = wt_create_shape(r, WT_REC);
//		wt_world_add_shape(w, s);
//
//		b = wt_create_body();
//		b->mas = 10;
//		b->pos.x = -100;
//		b->vel.x = 20;
//		b->u = 0.5;
//		//b->angular = 100.0f / 180.0f * WT_PI;
//		r = wt_create_rect(b, 2, 2);
//		s = wt_create_shape(r, WT_REC);
//		wt_world_add_shape(w, s);

//		b1 = wt_create_body();
//		b1->mas = 10;
//		b1->pos.y = 0;
//		b1->pos.x = 0;
//		b1->u=0.5;
//		c = wt_create_cir(b1, 5);
//		s = wt_create_shape(c, WT_CIR);
//		wt_world_add_shape(w, s);
//
//		b2 = wt_create_body();
//		b2->mas = 10;
//		b2->pos.y = 0;
//		b2->pos.x = 15;
//		b2->u=0.5;
//		c = wt_create_cir(b2, 5);
//		s = wt_create_shape(c, WT_CIR);
//		wt_world_add_shape(w, s);
//
//		wt_spring *spr = wt_create_spring(b1,b2,40,30,20,0.01);
//		wt_world_add_spring(w,spr);
	wt_spring_test();
	//wt_wall_test();
}

void wt_generate_liquid(wt_world *w){
	//wt_liquid * l = wt_create_liquid();
	//wt_array_add(w->liquid,l);


}

void runPhy() {
	w = wt_create_world();
	wt_generate_body(w);
	wt_generate_liquid(w);
	wt_gl_main(&run);
}

int main(int argc, char *argv[]) {
	//glutInit(&argc, argv);
	runPhy();
}

#include "wt_gl_draw.h"

void wt_draw_cir(wt_circle cir, wt_gl_color c)
{
    glColor4f(c.r, c.g, c.b, 1.0f);
    glVertexPointer(2, GL_FLOAT, 0, wt_cir_data);
    glPushMatrix();
    glTranslatef(cir.body->pos.x, cir.body->pos.y, 0.0f);
    glRotatef(cir.body->angular * 180.0f / WT_PI, 0.0f, 0.0f, 1.0f);
    glScalef(cir.radius, cir.radius, 1.0f);
    glDrawArrays(GL_LINE_STRIP, 0, wt_cir_count);
    glPopMatrix();
}

void wt_draw_shapes(wt_array *shapes)
{
    wt_gl_color c;
    c.r = 0.06; c.g = 0.9; c.b = 0.221;

    wt_circle *cir;
    //LOGI("draw shaps num: %d",shapes->num);
    for (int i = 0; i < shapes->num; ++i)
    {
        wt_shape *s = shapes->array[i];
        if (s->type == WT_CIR)
        {
        	//LOGI("draw shaps");
            cir = (wt_circle *) (s->shape);
            wt_draw_cir(*cir, c);
        }
    }
}

void wt_draw(wt_world *w)
{

    //wt_begin_draw();
    

    //wt_draw_background(w->width);
	LOGI("wt_draw");
    wt_array *shapes = w->shapes;
    wt_draw_shapes(w->shapes);

    // glEnable(GL_ALPHA_TEST);
    // glAlphaFunc(GL_GEQUAL, 0.01);
    // glBindTexture(GL_TEXTURE_2D, texture_ID_list[0]);
    // wt_draw_fluid(w->fluid);
    // glDisable(GL_ALPHA_TEST);

    //wt_end_draw();
}
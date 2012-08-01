#include "wt_gl.h"

void wt_draw_cir(wt_circle cir, wt_gl_color c)
{
    //printf("ang:%f radius:%f\n", cir.body->angular, cir.radius );
    glColor4f(c.r, c.g, c.b, 1.0f);
    glVertexPointer(2, GL_FLOAT, 0, wt_cir_data);
    glPushMatrix();
    glTranslatef(cir.body->pos.x, cir.body->pos.y, 0.0f);
    glRotatef(cir.body->angular * 180.0f / WT_PI, 0.0f, 0.0f, 1.0f);
    glScalef(cir.radius, cir.radius, 1.0f);
    //GL_TRIANGLE_FAN GL_LINE_STRIP
    glDrawArrays(GL_LINE_STRIP, 0, wt_cir_count);
    glPopMatrix();
}

// void wt_draw_rect(wt_rect rect, wt_gl_color c)
// {
//     //printf("ang:%f radius:%f\n", cir.body->angular, cir.radius );
//     glColor4f(c.r, c.g, c.b, 1.0f);
//     glVertexPointer(2, GL_FLOAT, 0, wt_rect_data);
//     glPushMatrix();
//     glTranslatef(rect.body->pos.x, rect.body->pos.y, 0.0f);
    
//     glScalef(rect.r_len*2, rect.u_len*2, 1.0f);
//     glRotatef(rect.body->angular * 180.0f / WT_PI, 0.0f, 0.0f, 1.0f);
//     //GL_TRIANGLE_FAN GL_LINE_STRIP
//     glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
//     glPopMatrix();
// }
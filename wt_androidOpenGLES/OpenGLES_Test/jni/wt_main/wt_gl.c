#include "wt_gl.h"

void wt_draw_dot2f(float x, float y)
{
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glLoadIdentity();
    glColor3f(1.0f, 0.5f, 0.5f);
    glPointSize(5.0f);
    glPushMatrix();
    glTranslatef( x,  y, 0.0f);
    glBegin(GL_POINTS);
    glVertex2f(0, 0);
    glEnd();
    glPopMatrix();
    //glutSwapBuffers();
}
void wt_draw_dot(wt_vec p, wt_r32 size, wt_gl_color c)
{
    glColor3f(1.0f, 0.5f, 0.5f);
    glPointSize(size);
    //glColor3f(c.r,c.g,c.b);
    glBegin(GL_POINTS);
    glVertex2f(p.x, p.y);
    glEnd();
    printf("x:%f,y:%f,c.r:%f\n", p.x, p.y, c.r);
}
void wt_draw_line()
{
    glColor3f(1.0f, 0.5f, 0.5f);
    glPushMatrix();
    glScalef(0.5f, 0.5f, 1.0f);
    glBegin(GL_LINES);
    glVertex2f(0, 0.5);
    glVertex2f(0, 1);
    glEnd();
    glPopMatrix();
}
void wt_draw_cir(wt_circle cir, wt_gl_color c)
{
    //printf("ang:%f radius:%f\n", cir.body->angular, cir.radius );
    glColor3f(c.r, c.g, c.b);
    glVertexPointer(2, GL_FLOAT, 0, wt_cir_data);
    glPushMatrix();
    glTranslatef(cir.body->pos.x, cir.body->pos.y, 0.0f);
    glRotatef(cir.body->angular * 180.0f / WT_PI, 0.0f, 0.0f, 1.0f);
    glScalef(cir.radius, cir.radius, 1.0f);
    //GL_TRIANGLE_FAN GL_LINE_STRIP
    glDrawArrays(GL_LINE_STRIP, 0, wt_cir_count);
    glPopMatrix();
}

void wt_draw_rect(wt_rect rect, wt_gl_color c)
{
    glColor3f(c.r, c.g, c.b);
    glPushMatrix();
    glTranslatef(rect.body->pos.x, rect.body->pos.y, 0.0f);
    glRotatef(rect.body->angular * 180.0f / WT_PI, 0.0f, 0.0f, 1.0f);
    glRectf(-rect.r_len, -rect.u_len, rect.r_len, rect.u_len);
    glPopMatrix();
}
//--------------------------------------------------------------------------
void wt_gl_init(GLvoid)
{
    GLfloat values[2];
    glGetFloatv(GL_LINE_WIDTH_GRANULARITY, values);
    glGetFloatv(GL_LINE_WIDTH_RANGE, values);
    //glEnable(GL_POINT_SMOOTH);                          //点抗锯齿
    //glEnable(GL_LINE_SMOOTH);                         //线抗锯齿
    //glEnable(GL_POLYGON_SMOOTH);                      //多边形抗锯齿
    glEnableClientState(GL_VERTEX_ARRAY);               //开启画array功能
    glEnable(GL_BLEND);                                 //启用混合
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);          //设置抗锯齿的参数
    glClearColor(1.0, 1.0, 1.0, 1.0);                   //设置背景颜色
    //gluOrtho2D(-100.0,100.0,-100.0,100.0);
}
void wt_gl_reshape ( int w, int h )   // Create The Reshape Function (the viewport)
{
	     glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	     glMatrixMode(GL_PROJECTION);
	     glLoadIdentity();
	     //gluPerspective(120.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
	     gluOrtho2D(-100, 100, -100,100);
	     glMatrixMode(GL_MODELVIEW);
	     glLoadIdentity();
	     glTranslatef(0,0,-15.0);
}

void wt_gl_main(void (*wt_gl_display)())
{
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    //glutInitWindowPosition(50, 50);
    glutInitWindowPosition(400, 600);
    glutInitWindowSize(500, 500);
    glutCreateWindow("waterZ");
    wt_gl_init ();//因为里面的抗锯齿,需要在创建窗口后调用才行
    glutReshapeFunc(wt_gl_reshape);
    glutDisplayFunc(wt_gl_display);
    glutIdleFunc(wt_gl_display);
    glutMainLoop();
}

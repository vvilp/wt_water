#include    <stdio.h>                           // 标准输入/输出库的头文件
#include    "dll/glaux.h"                           // GLaux库的头文件
#include    "dll/glut.h"
#include "wt_meta_ball.h"

GLuint texture_ID_list[10];
const int len = 200 ;
int mousex = 0;
int mousey = 0;
void wt_draw_dot2f(float x, float y)
{
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glLoadIdentity();
    glColor3f(1.0f, 0.5f, 0.5f);
    glPointSize(10.0f);
    glPushMatrix();
    glTranslatef( x,  y, 0.0f);
    glBegin(GL_POINTS);
    glVertex2f(0, 0);
    glEnd();
    glPopMatrix();
    //glutSwapBuffers();
}
void wt_draw_background(float width)
{
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_ID_list[0]);
    glScalef(width, width, 1.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -1.0f,   -1.0f,   -1.0f);  // 纹理和四边形的左下
    glTexCoord2f(2.0f, 0.0f); glVertex3f(  1.0f,   -1.0f,   -1.0f);  // 纹理和四边形的右下
    glTexCoord2f(2.0f, 2.0f); glVertex3f(  1.0f,  1.0f,   -1.0f);    // 纹理和四边形的右上
    glTexCoord2f(0.0f, 2.0f); glVertex3f( -1.0f,  1.0f,   -1.0f);    // 纹理和四边形的左上
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

int wt_gener_image_data() //自己绘制纹理
{
    glGenTextures(2, &texture_ID_list[0]);
    glBindTexture(GL_TEXTURE_2D, texture_ID_list[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, map_width, map_width, 0, GL_RGBA , GL_UNSIGNED_BYTE, meta_map); //速度较慢所以在初始化的时候用
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
}
void wt_gener_meta_ball()
{
    wt_meta_map_init(100);
    wt_meta_map_add_cir(10,10,5);
    wt_meta_map_add_cir(15,15,5);
    wt_meta_map_add_cir(25,25,5);
    wt_meta_map_add_cir(mousex,mousey,5);
    wt_meta_map_set_color();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, map_width, map_width, 0, GL_RGBA , GL_UNSIGNED_BYTE, meta_map); 
    //wt_gener_image_data(); 
}


void run()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //可以正常使用shape函数
    glLoadIdentity(); //初始化原点坐标
    //glEnable(GL_ALPHA_TEST);
    //glAlphaFunc(GL_GEQUAL, 0.6);
    //glAlphaFunc(GL_LEQUAL, 0.7);
    //wt_gener_meta_ball();
    wt_gener_meta_ball();
    wt_draw_background(100);

    //wt_draw_dot2f(10, 10);
    glutSwapBuffers();
}

void wt_gl_reshape1 ( int w, int h )   // Create The Reshape Function (the viewport)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(120.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
    //gluPerspective(45.0f,(GLfloat)w/(GLfloat)h,0.1f,100.0f);
    gluOrtho2D(0, 100, 0, 100); //左下角x坐标，右上角x坐标，左下角y坐标，右上角y坐标
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}








int InitGL()                                // 此处开始对OpenGL进行所有设置
{

    //wt_gener_meta_ball();
    wt_gener_image_data();
    glEnable(GL_TEXTURE_2D);                       // 启用纹理映射
    GLfloat values[2];
    glGetFloatv(GL_LINE_WIDTH_GRANULARITY, values);
    glGetFloatv(GL_LINE_WIDTH_RANGE, values);
    glEnableClientState(GL_VERTEX_ARRAY);               //开启画array功能
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_BLEND);                                 //启用混合
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_ALPHA_TEST);
    //glAlphaFunc(GL_GEQUAL, 0.5);
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);          //设置抗锯齿的参数
    glClearColor(1.0, 1.0, 1.0, 1.0);                   //设置背景颜色
    return TRUE;                                // 初始化 OK
}



void mouseMove(int x,int y){
    //
    float tmp_x = (float) x;
    float tmp_y = (float) y;
    tmp_x = (float)x / 400 * 100;
    tmp_y = (float)y / 400 * 100;
    mousex = tmp_x;
    mousey = 100 - tmp_y;
    //printf("%d %d\n",mousex,mousey );

}

int main()
{
    GLubyte a = 255;
    int b = a -100;

    if(a + (GLubyte)100 > 255) {
        GLubyte c = a + (GLubyte)100;
        printf("%d\n",c);
    }
    
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowPosition(350, 350);
    glutInitWindowSize(400, 400);
    glutCreateWindow("water-WZT");
    InitGL();
    glutReshapeFunc(wt_gl_reshape1);
    glutDisplayFunc(run);
    glutIdleFunc(run);
    glutMotionFunc(mouseMove);
    glutPassiveMotionFunc(mouseMove);
    glutMainLoop();
}
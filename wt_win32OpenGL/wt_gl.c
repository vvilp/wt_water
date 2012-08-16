#include "wt_gl.h"


GLubyte Texture[200][200][4];//白色渐变球
GLubyte Texture1[200][200][4];//黑色
int texture_ID_list[3];
const int len = 200 ;
float Falloff(float distance, float maxDistance, float scalingFactor)
{
    if (distance <= maxDistance / 3)
    {
        return scalingFactor * (1 - 3 * distance * distance / (maxDistance * maxDistance));
    }
    else if (distance <= maxDistance)
    {
        float x = 1 - distance / maxDistance;
        return (3.0f / 2.0f) * scalingFactor * x * x;
    }
    else
        return 0;
}
int wt_gener_image_data() //自己绘制纹理
{
    int len = 200;
    memset(Texture, 0, sizeof(Texture));
    memset(Texture1, 0, sizeof(Texture));
    for (int x = 0 ; x < len ; x++)
    {
        for (int y = 0 ; y < len ; y++)
        {
            //float distance = sqrt(x*x + y*y) ;
            //float alpha = Falloff(distance,10,1);

            // Texture[x][y][0] = 150;
            // Texture[x][y][1] = 98;
            // Texture[x][y][2] = 238;
            // Texture[x][y][3] = 255;

            Texture[x][y][0] = 255;
            Texture[x][y][1] = 255;
            Texture[x][y][2] = 255;
            Texture[x][y][3] = 255;

            Texture1[x][y][0] = 255;
            Texture1[x][y][1] = 0;
            Texture1[x][y][2] = 0;
            Texture1[x][y][3] = 255;

            float alpha = Falloff(sqrt((x - len / 2) * (x - len / 2) + (y - len / 2) * (y - len / 2)), len / 2, 1);
            //wt_debug("alpha %f\n", alpha);
            // if(sqrt( (x-100)*(x-100)+(y-100)*(y-100) ) < 100){

            //     Texture[x][y][3] = 255;
            // }else{
            //     Texture[x][y][3] = 0;
            // }

            Texture[x][y][0] = wt_rclamp(alpha * 256 - 20, 0, 255);
            Texture[x][y][1] = wt_rclamp(alpha * 256 - 20, 0, 255);
            Texture[x][y][3] = wt_rclamp(alpha * 256 + 0.5f, 0, 255);

            //wt_debug("alpha : %d , alpha : %f\n", Texture[x][y][3],alpha);
            //system("pause");
        }
    }

    //GL_RGBA 表示 RGB + alpha ,将纹理存到缓存中
    glGenTextures(2, &texture_ID_list[0]);
    glBindTexture(GL_TEXTURE_2D, texture_ID_list[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, len, len, 0, GL_RGBA , GL_UNSIGNED_BYTE, Texture); //速度较慢所以在初始化的时候用

    //glBindTexture(GL_TEXTURE_2D, texture_ID_list[1]);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, len, len, 0, GL_RGBA , GL_UNSIGNED_BYTE, Texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
}

AUX_RGBImageRec *LoadBMP(char *Filename)                    // 载入位图图象
{
    FILE *File = NULL;                          // 文件句柄
    if (!Filename)                              // 确保文件名已提供
    {
        return NULL;                            // 如果没提供，返回 NULL
    }
    File = fopen(Filename, "r");                    // 尝试打开文件
    if (File)                               // 文件存在么?
    {
        fclose(File);                           // 关闭句柄
        return auxDIBImageLoad(Filename);               // 载入位图并返回指针
    }
    return NULL;                                // 如果载入失败，返回 NULL
}

int wt_load_bmp()                                // 载入位图(调用上面的代码)并转换成纹理
{
    printf("LoadGLTextures()\n");
    int Status = FALSE;                         // 状态指示器
    AUX_RGBImageRec *TextureImage[1];                   // 创建纹理的存储空间
    memset(TextureImage, 0, sizeof(void *) * 1);            // 将指针设为 NULL
    if (TextureImage[0] = LoadBMP("4.bmp"))
    {
        Status = TRUE;                          // 将 Status 设为 TRUE
        glGenTextures(3, &texture_ID_list[2]);                  // 创建纹理
        glBindTexture(GL_TEXTURE_2D, texture_ID_list[2]);// 使用来自位图数据生成 的典型纹理
        glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);// 生成纹理
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    // if (TextureImage[0])                         // 纹理是否存在
    // {
    //  if (TextureImage[0]->data)                  // 纹理图像是否存在
    //  {
    //      free(TextureImage[0]->data);                // 释放纹理图像占用的内存
    //  }

    //  free(TextureImage[0]);                      // 释放图像结构
    // }

    return Status;                              // 返回 Status
}

void texture_colorkey()
{
    GLint width, height;
    GLubyte *pixels = 0;

    // 获得纹理的大小信息
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

    // 分配空间并获得纹理像素
    pixels = (GLubyte *)malloc(width * height * 4);
    if ( pixels == 0 )
        return;
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    // 修改像素中的Alpha值
    // 其中pixels[i*4], pixels[i*4+1], pixels[i*4+2], pixels[i*4+3]
    //    分别表示第i个像素的蓝、绿、红、Alpha四种分量，0表示最小，255表示最大
    {
        GLint i;
        GLint count = width * height;
        for (i = 0; i < count; ++i)
        {
            if ( abs(pixels[i * 4]) >= 200
                    && abs(pixels[i * 4 + 1]) >= 200
                    && abs(pixels[i * 4 + 2]) >= 200 )
                pixels[i * 4 + 3] = 0;
            else
                pixels[i * 4 + 3] = 255;
        }
    }

    // 将修改后的像素重新设置到纹理中，释放内存
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    free(pixels);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}


void wt_draw_dot2f(float x, float y)
{
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glLoadIdentity();

    glPushMatrix();
    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TEXTURE_BIT);
    glColor3f(100.0 / 255, 0, 0);
    glPointSize(10.0f);

    glTranslatef( x,  y, 0.0f);
    glBegin(GL_POINTS);
    glVertex2f(0, 0);
    glEnd();
    glPopAttrib();
    glPopMatrix();
    //glutSwapBuffers();
}
void wt_draw_dot(wt_vec p, wt_r32 size, wt_gl_color c)
{
    //glColor3f(1.0f, 0.5f, 0.5f);
    glPointSize(size);
    glColor3f(c.r, c.g, c.b);
    glBegin(GL_POINTS);
    glVertex2f(p.x, p.y);
    glEnd();
    //printf("x:%f,y:%f,c.r:%f\n", p.x, p.y, c.r);
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

void wt_draw_cir_texture(wt_circle cir, wt_gl_color c)
{
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_ID_list[2]);
    glTranslatef(cir.body->pos.x, cir.body->pos.y, 0.0f); //绘制这种纹理,pos在左下角

    glScalef(cir.radius, cir.radius, 1.0f);
    glRotatef(cir.body->angular * 180.0f / WT_PI, 0.0f, 0.0f, 1.0f);
    //glBindTexture(GL_TEXTURE_2D, texture_ID_list[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -1.0f,   -1.0f,   0.0f);  // 纹理和四边形的左下
    glTexCoord2f(1.0f, 0.0f); glVertex3f(  1.0f,   -1.0f,   0.0f);  // 纹理和四边形的右下
    glTexCoord2f(1.0f, 1.0f); glVertex3f(  1.0f,  1.0f,   0.0f);    // 纹理和四边形的右上
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -1.0f,  1.0f,   0.0f);    // 纹理和四边形的左上
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void wt_draw_cir_simply(wt_circle cir, wt_gl_color c)
{
    //printf("ang:%f radius:%f\n", cir.body->angular, cir.radius );
    glColor3f(c.r, c.g, c.b);
    glVertexPointer(2, GL_FLOAT, 0, wt_cir_data);
    glPushMatrix();
    glTranslatef(cir.body->pos.x, cir.body->pos.y, 0.0f);
    glRotatef(cir.body->angular * 180.0f / WT_PI, 0.0f, 0.0f, 1.0f);
    //glRotatef(cir.body->angular, 0.0f, 0.0f, 1.0f);
    glScalef(cir.radius, cir.radius, 1.0f);
    //GL_TRIANGLE_FAN GL_LINE_STRIP
    glDrawArrays(GL_LINE_STRIP, 0, wt_cir_count);
    glPopMatrix();
}

void wt_draw_cir(wt_circle cir, wt_gl_color c)
{
    //wt_draw_cir_simply(cir, c);
    wt_draw_cir_texture(cir, c);
}



void wt_draw_shapes(wt_array *shapes)
{
    wt_gl_color c;
    c.r = 0.06; c.g = 0.9; c.b = 0.221;

    wt_circle *cir;
    for (int i = 0; i < shapes->num; ++i)
    {
        wt_shape *s = shapes->array[i];
        if (s->type == WT_CIR)
        {
            cir = (wt_circle *) (s->shape);
            wt_draw_cir(*cir, c);
        }
    }
}

// void wt_draw_partical(wt_partical p, wt_gl_color c)
// {
//     // glColor3f(c.r, c.g, c.b);
//     // wt_draw_dot(p.pos, p.radius, c);
//     glPushMatrix();
//     glEnable(GL_TEXTURE_2D);
//     glTranslatef(p.pos.x - p.radius / 2, p.pos.y - p.radius / 2, 0.0f); //绘制这种纹理,pos在左下角
//     glScalef(p.radius, p.radius, 0.0f);
//     //glBindTexture(GL_TEXTURE_2D, texture_ID_list[0]);
//     glBegin(GL_QUADS);
//     glTexCoord2f(0.0, 0.0); glVertex3f( 0.0,   0.0,   0.0);  // 纹理和四边形的左下
//     glTexCoord2f(1.0, 0.0); glVertex3f( 1.0,   0.0,   0.0);  // 纹理和四边形的右下
//     glTexCoord2f(1.0, 1.0); glVertex3f( 1.0,   1.0,   0.0);    // 纹理和四边形的右上
//     glTexCoord2f(0.0, 1.0); glVertex3f( 0,     1.0,   0.0);    // 纹理和四边形的左上
//     glEnd();
//     glDisable(GL_TEXTURE_2D);
//     glPopMatrix();

// }

void wt_draw_fluid_body(wt_body p, wt_gl_color c)
{

    glColor3f(c.r, c.g, c.b);
    wt_draw_dot(p.pos, 2, c);
    // wt_r32 radius = 5;
    // glPushMatrix();
    // glEnable(GL_TEXTURE_2D);
    // glTranslatef(p.pos.x - radius / 2, p.pos.y - radius / 2, 0.0f); //绘制这种纹理,pos在左下角
    // glScalef(radius, radius, 0.0f);
    // //glBindTexture(GL_TEXTURE_2D, texture_ID_list[0]);
    // glBegin(GL_QUADS);
    // glTexCoord2f(0.0, 0.0); glVertex3f( 0.0,   0.0,   0.0);  // 纹理和四边形的左下
    // glTexCoord2f(1.0, 0.0); glVertex3f( 1.0,   0.0,   0.0);  // 纹理和四边形的右下
    // glTexCoord2f(1.0, 1.0); glVertex3f( 1.0,   1.0,   0.0);    // 纹理和四边形的右上
    // glTexCoord2f(0.0, 1.0); glVertex3f( 0,     1.0,   0.0);    // 纹理和四边形的左上
    // glEnd();
    // glDisable(GL_TEXTURE_2D);
    // glPopMatrix();


}



void wt_draw_fluid(wt_pvf_fluid *fluid)
{
    wt_gl_color c;
    c.r = 58.0 / 255.0; c.g = 72.0 / 255.0; c.b = 243.0 / 255.0;
    wt_array *pvf_particals = fluid->pvf_particals;
    for (int i = 0 ; i < pvf_particals->num; i++)
    {
        wt_pvf_partical *pvf_p = pvf_particals->array[i];
        wt_draw_fluid_body(*(pvf_p->body), c);
    }
}

void wt_begin_draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //可以正常使用shape函数
    glLoadIdentity(); //初始化原点坐标
    glPushMatrix();
    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TEXTURE_BIT);
    glDisable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);                         //启用混合
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);


}

void wt_end_draw()
{
    glPopAttrib();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glutSwapBuffers();
}

void wt_draw(wt_world *w)
{

    wt_begin_draw();
    wt_array *shapes = w->shapes;

    //wt_draw_dot2f(50, 50);

    glPushMatrix();
    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TEXTURE_BIT);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GEQUAL, 0.5);
    glBindTexture(GL_TEXTURE_2D, texture_ID_list[0]);
    //glAlphaFunc(GL_LEQUAL, 0.9);
    wt_draw_fluid(w->fluid);
    glDisable(GL_ALPHA_TEST);
    wt_gl_color c;
    c.r = 58.0 / 255.0; c.g = 72.0 / 255.0; c.b = 243.0 / 255.0;
    glPopAttrib();
    glPopMatrix();

    wt_draw_shapes(w->shapes);

    // //Draw single blue quad (background)
    // glDisable(GL_TEXTURE_2D);
    // glColor3f(c.r,c.g,c.b);
    // glBegin(GL_QUADS);
    // glVertex3f( 0.0,   0.0,   0.0);  // 纹理和四边形的左下
    // glVertex3f( 100.0,   0.0,   0.0);  // 纹理和四边形的右下
    // glVertex3f( 100.0,   100.0,   0.0);    // 纹理和四边形的右上
    // glVertex3f( 0.0,   100.0,   0.0);    // 纹理和四边形的左上
    // glEnd();


    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, len, len, 0, GL_RGBA , GL_UNSIGNED_BYTE, Texture1);
    // glEnable(GL_TEXTURE_2D);
    // //glBindTexture(GL_TEXTURE_2D, Texture);               // 选择纹理
    // //glTranslatef(p.pos.x - p.radius / 2, p.pos.y - p.radius / 2, 0.0f); //绘制这种纹理,pos在左下角
    // //glScalef(p.radius, p.radius, 0.0f);
    // glBegin(GL_QUADS);
    // glTexCoord2f(0.0, 0.0); glVertex3f( 0.0,   0.0,   0.0);  // 纹理和四边形的左下
    // glTexCoord2f(1.0, 0.0); glVertex3f( 100.0,   0.0,   0.0);  // 纹理和四边形的右下
    // glTexCoord2f(1.0, 1.0); glVertex3f( 100.0,   100.0,   0.0);    // 纹理和四边形的右上
    // glTexCoord2f(0.0, 1.0); glVertex3f( 0,     100.0,   0.0);    // 纹理和四边形的左上
    // glEnd();
    // glDisable(GL_TEXTURE_2D);
    // glDisable(GL_ALPHA_TEST);


    wt_end_draw();
}


// void wt_draw_rect(wt_rect rect, wt_gl_color c)
// {
//     glColor3f(c.r, c.g, c.b);
//     glPushMatrix();
//     glTranslatef(rect.body->pos.x, rect.body->pos.y, 0.0f);
//     glRotatef(rect.body->angular * 180.0f / WT_PI, 0.0f, 0.0f, 1.0f);
//     glRectf(-rect.r_len, -rect.u_len, rect.r_len, rect.u_len);
//     glPopMatrix();
// }
//--------------------------------------------------------------------------

int wt_loadGLTextures() //自己绘制纹理
{

    GLubyte Texture[200][200][4];
    int len = 200;
    memset(Texture, 0, sizeof(Texture));

    for (int x = 0 ; x < len ; x++)
    {
        for (int y = 0 ; y < len ; y++)
        {
            //float distance = sqrt(x*x + y*y) ;
            //float alpha = Falloff(distance,10,1);

            // Texture[x][y][0] = 150;
            // Texture[x][y][1] = 98;
            // Texture[x][y][2] = 238;
            // Texture[x][y][3] = 255;

            Texture[x][y][0] = 255;
            Texture[x][y][1] = 255;
            Texture[x][y][2] = 255;
            Texture[x][y][3] = 255;

            float alpha = Falloff(sqrt((x - len / 2) * (x - len / 2) + (y - len / 2) * (y - len / 2)), len / 2, 1);
            Texture[x][y][3] = wt_rclamp(alpha * 256 + 0.5f, 0, 255);
        }
    }

    //GL_RGBA 表示 RGB + alpha
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, len, len, 0, GL_RGBA , GL_UNSIGNED_BYTE, Texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // 线形滤波
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );  // 线形滤波
}

//将纹理映射到圆形上,不是特别好
// void wt_draw_cir_addImage()
// {
//     glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//     glBindTexture(GL_TEXTURE_2D, texture[0]);
//     glVertexPointer(2, GL_FLOAT, 0, wt_cir_data);
//     glTexCoordPointer(2, GL_FLOAT, 0, wt_cir_data);
//     glPushMatrix();
//     glTranslatef(10, 10, 0.0f);
//     glScalef(10, 10, 0.0f);
//     glDrawArrays(GL_TRIANGLE_FAN, 0, wt_cir_count);
//     glPopMatrix();
// }

void wt_gl_init(GLvoid)
{
    wt_gener_image_data();
    wt_load_bmp();
    texture_colorkey();
    //glEnable(GL_TEXTURE_2D);
    GLfloat values[2];
    glGetFloatv(GL_LINE_WIDTH_GRANULARITY, values);
    glGetFloatv(GL_LINE_WIDTH_RANGE, values);
    //glEnable(GL_POINT_SMOOTH);                          //点抗锯齿
    //glEnable(GL_LINE_SMOOTH);                         //线抗锯齿
    //glEnable(GL_POLYGON_SMOOTH);                      //多边形抗锯齿
    glEnableClientState(GL_VERTEX_ARRAY);               //开启画array功能
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    //glEnable(GL_ALPHA_TEST);
    //glAlphaFunc(GL_EQUAL, 0.7); //0是透明 大于是通过测试
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);          //设置抗锯齿的参数
    glClearColor(1.0, 1.0, 1.0, 1.0);                   //设置背景颜色
    //glClearColor(0.0f, 0.0f, 0.0f, 0.5f); //黑色
    //gluOrtho2D(-100.0,100.0,-100.0,100.0);

}
void wt_gl_reshape ( int w, int h )   // Create The Reshape Function (the viewport)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(120.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
    gluOrtho2D(0, 100, 0, 100); //左下角x坐标，右上角x坐标，左下角y坐标，右上角y坐标
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, -15.0);
}


void wt_gl_main(void (*wt_gl_key_fun)(unsigned char c, __attribute__((unused)) int x, __attribute__((unused))  int y), void (*wt_gl_display)())
{
    // glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    //    //glutInitWindowPosition(50, 50);
    //    glutInitWindowPosition(350, 350);
    //    glutInitWindowSize(400, 400);
    //    glutCreateWindow("waterZ");
    //    wt_gl_init ();//因为里面的抗锯齿,需要在创建窗口后调用才行

    //    glutReshapeFunc(wt_gl_reshape);
    //    glutDisplayFunc(wt_gl_display);
    // //   glutKeyboardFunc(keyboard1);
    //    glutIdleFunc(wt_gl_display);
    //    glutMainLoop();
}

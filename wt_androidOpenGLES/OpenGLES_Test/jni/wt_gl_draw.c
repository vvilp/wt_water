#include "wt_gl_draw.h"

//有一个重要结论，无论在何时，纹理一定要用2的倍数
GLubyte Texture[128][128][4];//白色渐变球
GLubyte Texture1[128][128][4];//黑色
int texture_ID_list[10];
const int len = 128 ;
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
    memset(Texture, 0, sizeof(Texture));
    memset(Texture1, 0, sizeof(Texture));
    for (int x = 0 ; x < len ; x++)
    {
        for (int y = 0 ; y < len ; y++)
        {

            Texture[x][y][0] = 46;
            Texture[x][y][1] = 56;
            Texture[x][y][2] = 250;
            Texture[x][y][3] = 255;
            float alpha = Falloff(sqrt((x - len / 2) * (x - len / 2) + (y - len / 2) * (y - len / 2)), len / 2, 1);
            Texture[x][y][3] = wt_rclamp(alpha * 256 + 0.5f, 0, 255);
        }
    }
    glGenTextures(2, &texture_ID_list[0]);
    glBindTexture(GL_TEXTURE_2D, texture_ID_list[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, len, len, 0, GL_RGBA , GL_UNSIGNED_BYTE, Texture); //速度较慢所以在初始化的时候用
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
}

void wt_draw_dot(wt_vec p, wt_r32 size, wt_gl_color c)
{

    glColor4f(c.r, c.g, c.b, 1.0f);
    glVertexPointer(2, GL_FLOAT, 0, wt_rect_data);
    glPushMatrix();
    glTranslatef(p.x, p.y, 0.0f);
    //glRotatef(cir.body->angular * 180.0f / WT_PI, 0.0f, 0.0f, 1.0f);
    glScalef(1, 1, 1.0f);
    glDrawArrays(GL_LINE_STRIP, 0, wt_rect_count);
    glPopMatrix();

}
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

void wt_draw_fluid_body(wt_body p, wt_gl_color c)
{

    //wt_draw_dot(p.pos, 2, c);

    wt_r32 radius = 10;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_ID_list[0]);
    glVertexPointer(2, GL_FLOAT, 0, wt_rect_data);
    glTexCoordPointer(2, GL_FLOAT, 0, wt_rect_data);
    glPushMatrix();
    glTranslatef(p.pos.x - radius / 2, p.pos.y - radius / 2, 0.0f); //绘制这种纹理,pos在左下角
    glScalef(radius, radius, 1.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, wt_rect_count);
    glPopMatrix();
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
    glDisable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);                         //启用混合
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glColor4f( 1.0f,1.0f,1.0f,0.0); //清除其他颜色，显示纹理本身颜色

}

void wt_end_draw()
{
    glMatrixMode(GL_MODELVIEW);
    //glutSwapBuffers();
}
void wt_draw(wt_world *w)
{

    //wt_begin_draw();

    //wt_draw_background(w->width);
    //LOGI("wt_draw");
    wt_array *shapes = w->shapes;
    wt_draw_shapes(w->shapes);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_ALPHA_TEST);
    //glAlphaFunc(GL_GEQUAL, 0.01);
    glBindTexture(GL_TEXTURE_2D, texture_ID_list[0]);
    wt_draw_fluid(w->fluid);
    //glDisable(GL_ALPHA_TEST);

    //wt_end_draw();
}



void wt_gl_init()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    wt_gener_image_data();
}
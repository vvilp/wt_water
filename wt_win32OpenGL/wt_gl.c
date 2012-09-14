
#include "wt_gl.h"


GLubyte Texture[200][200][4];//白色渐变球
GLubyte Texture1[200][200][4];//黑色
int texture_ID_list[10];
const int len = 200 ;

GLuint   program_object;  // a handler to the GLSL program used to update
GLuint   vertex_shader;   // a handler to vertex shader. This is used internally
GLuint   fragment_shader; // a handler to fragment shader. This is used internally too

static const char *vertex_source =
{
    "void main(){"
    "gl_Position = gl_Vertex;"
    "}"
};

// a simple fragment shader source
// this change the fragment's color by yellow color
static const char *fragment_source =
{
    "\
    uniform float window_width;\
    uniform float world_width;\
    uniform int cir_num;\
    uniform vec3 cir[128];\
    vec3 world_to_win_size(vec3 cir) {\
        return cir / world_width * window_width;\
    }\
    float meta_falloff(float dis_sq, float max_dis_sq)\
    {\
        if(dis_sq < max_dis_sq){\
            float x = 1 - dis_sq / max_dis_sq;\
            return (3.0f / 2.0f) * x * x;\
        }else{\
            return 0.0;\
        }\
    }\
    void main(){\
        float threhold = 0;\
        for(int i = 0 ; i < cir_num ; i++){\
            vec3 c = world_to_win_size(cir[i]);\
            vec2 pos = gl_FragCoord.xy - c.xy;\
            float dist_squared = dot(pos, pos);\
            float alpha = meta_falloff(dist_squared,c.z*c.z);\
            threhold += clamp(alpha * 256, 0.0, 255.0);\
        }\
        if(threhold > 200 ){\
            gl_FragColor = vec4(255.0, 255.0, 255.0, 1.0);\
        }\
        else if (threhold > 150 && threhold < 200){\
            gl_FragColor = vec4(0.0, 128.0/255.0, 192.0/255.0, 0.6);\
        }\
        else {\
            gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);\
        }\
    }\
    "
};

int init_shader(void)
{
    //glutCreateWindow("GLSL Hello World!");          // Window Title

    glewInit();
    if (glewIsSupported("GL_VERSION_2_0"))
        printf("Ready for OpenGL 2.0\n");
    else
    {
        printf("OpenGL 2.0 not supported\n");
        exit(1);
    }
    //glClearColor(1.0, 1.0, 1.0, 1.0);

    vertex_shader   = glCreateShader(GL_VERTEX_SHADER);
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);


    glShaderSource(vertex_shader, 1, &vertex_source, NULL);
    glShaderSource(fragment_shader, 1, &fragment_source, NULL);

    glCompileShader(vertex_shader);
    glCompileShader(fragment_shader);

    program_object  = glCreateProgram();

    glAttachShader(program_object, vertex_shader);
    glAttachShader(program_object, fragment_shader);

    glLinkProgram(program_object);


    //glUseProgram(p);

    return 1;
}

void wt_draw_fluid_meta_ball(wt_pvf_fluid *fluid)
{
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear Screen And Depth Buffer
    //glLoadIdentity();                                   // Reset The Current Modelview Matrix

    glUseProgram(program_object);
    glUniform1f(glGetUniformLocation(program_object, "window_width"), window_size);
    glUniform1f(glGetUniformLocation(program_object, "world_width"), world_size);

    wt_array *pvf_particals = fluid->pvf_particals;
    GLfloat cir[1000][3];
    for (int i = 0 ; i < pvf_particals->num; i++)
    {
        wt_pvf_partical *pvf_p = pvf_particals->array[i];
        cir[i][0] = pvf_p->body->pos.x;
        cir[i][1] = pvf_p->body->pos.y;
        cir[i][2] = 5;
    }
    glUniform3fv(glGetUniformLocation(program_object, "cir"), pvf_particals->num, cir);
    glUniform1i(glGetUniformLocation(program_object, "cir_num"), pvf_particals->num);

    glBegin(GL_QUADS);
    glVertex3f(-1, -1, 0.0);
    glVertex3f(1, -1, 0.0);
    glVertex3f(1, 1, 0.0);
    glVertex3f(-1, 1, 0.0);
    glEnd();
    glUseProgram(0);
    //glutSwapBuffers( );
}


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

            Texture[x][y][0] = 0;
            Texture[x][y][1] = 255;
            Texture[x][y][2] = 255;
            Texture[x][y][3] = 255;

            float alpha = Falloff(sqrt((x - len / 2) * (x - len / 2) + (y - len / 2) * (y - len / 2)), len / 2, 1);
            Texture[x][y][3] = wt_rclamp(alpha * 256 + 0.5f, 0, 255);

        }
    }

    //GL_RGBA 表示 RGB + alpha ,将纹理存到缓存中
    glGenTextures(2, &texture_ID_list[0]);
    glBindTexture(GL_TEXTURE_2D, texture_ID_list[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, len, len, 0, GL_RGBA , GL_UNSIGNED_BYTE, Texture); //速度较慢所以在初始化的时候用
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
}

// AUX_RGBImageRec *LoadBMP(char *Filename)                    // 载入位图图象
// {
//     FILE *File = NULL;                          // 文件句柄
//     if (!Filename)                              // 确保文件名已提供
//     {
//         return NULL;                            // 如果没提供，返回 NULL
//     }
//     File = fopen(Filename, "r");                    // 尝试打开文件
//     if (File)                               // 文件存在么?
//     {
//         fclose(File);                           // 关闭句柄
//         return auxDIBImageLoad(Filename);               // 载入位图并返回指针
//     }
//     return NULL;                                // 如果载入失败，返回 NULL
// }

// int wt_load_bmp(char *filename, int index)                               // 载入位图(调用上面的代码)并转换成纹理
// {
//     printf("LoadGLTextures()\n");
//     int Status = FALSE;                         // 状态指示器
//     AUX_RGBImageRec *TextureImage[1];                   // 创建纹理的存储空间
//     memset(TextureImage, 0, sizeof(void *) * 1);            // 将指针设为 NULL
//     if (TextureImage[0] = LoadBMP(filename))
//     {
//         Status = TRUE;                          // 将 Status 设为 TRUE
        
//         glBindTexture(GL_TEXTURE_2D, texture_ID_list[index]);// 使用来自位图数据生成 的典型纹理
//         glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);// 生成纹理
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//     }
//     return Status;                              // 返回 Status
// }

int wt_load_bmp2(char *filename, int index)
{
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;     // Position in the file where the actual data begins
    unsigned int width, height;
    unsigned int imageSize;   // = width*height*3
    unsigned char *data;

    FILE *file = fopen(filename, "rb");
    if (!file)
    {
        printf("Image could not be opened\n");
        return 0;
    }
    if ( fread(header, 1, 54, file) != 54 ) // If not 54 bytes read : problem
    {
        printf("Not a correct BMP file\n");
        return 0;
    }
    if ( header[0] != 'B' || header[1] != 'M' )
    {
        printf("Not a correct BMP file\n");
        return 0;
    }
    dataPos    = *(int *) & (header[0x0A]);
    imageSize  = *(int *) & (header[0x22]);
    width      = *(int *) & (header[0x12]);
    height     = *(int *) & (header[0x16]);
    // Some BMP files are misformatted, guess missing information
    if (imageSize == 0)    imageSize = width * height * 3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos == 0)      dataPos = 54; // The BMP header is done that way
    // Create a buffer
    data = (unsigned char *)malloc(imageSize);//new unsigned char [imageSize];

    // Read the actual data from the file into the buffer
    fread(data, 1, imageSize, file);

    //Everything is in memory now, the file can be closed
    fclose(file);

    //glGenTextures(index, &texture_ID_list[index]);                  // 创建纹理
    glBindTexture(GL_TEXTURE_2D, texture_ID_list[index]);// 使用来自位图数据生成 的典型纹理

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);// 生成纹理
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

void texture_colorkey()
{
    GLint width, height;
    //GLubyte *pixels = 0;

    // 获得纹理的大小信息
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

    wt_debug("width : %d , height : %d \n", width , height );
    // 分配空间并获得纹理像素
    GLubyte Texture2[64][64][4];//黑色

    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, Texture2);
    // 修改像素中的Alpha值
    // 其中pixels[i*4], pixels[i*4+1], pixels[i*4+2], pixels[i*4+3]
    //    分别表示第i个像素的蓝、绿、红、Alpha四种分量，0表示最小，255表示最大
    {
        GLint i;
        GLint count = width * height;
        // for (i = 0; i < count; ++i)
        // {
        //     if ( abs(pixels[i * 4]) >= 255 && abs(pixels[i * 4 + 1]) >= 255 && abs(pixels[i * 4 + 2]) >= 255 )
        //         pixels[i * 4 + 3] = 0;
        //     else
        //         pixels[i * 4 + 3] = 255;
        // }

        wt_i32 len = width;
        //wt_debug("width :%f \n", width);
        for (int x = 0 ; x < width ; x++)
        {
            for (int y = 0 ; y < width ; y++)
            {

                //if((x - len / 2) * (x - len / 2) + (y - len / 2) * (y - len / 2) >= len * len / 4)

                //Texture2[x][y][3] = 0;
                float alpha = Falloff(sqrt((x - len / 2) * (x - len / 2) + (y - len / 2) * (y - len / 2)), len / 2 + 2, 20);
                //直接贴图 并且通过判断白色来定义透明， 会有锯齿，所以使用渐变的技术来进行一定意义上的抗锯齿
                Texture2[x][y][3] = wt_rclamp(alpha * 256 + 0.5f, 0, 255);

            }
        }
    }

    // 将修改后的像素重新设置到纹理中，释放内存
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, Texture2);
    //free(pixels);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

void wt_draw_background(wt_r32 width)
{
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_ID_list[6]);
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

    glBegin(GL_POINTS);
    glColor3f(c.r, c.g, c.b);
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
    // glEnable(GL_ALPHA_TEST);
    // glAlphaFunc(GL_GEQUAL, 0.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_ID_list[5]);
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
    //glDisable(GL_ALPHA_TEST);
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


void wt_draw_fluid_body(wt_body p, wt_gl_color c)
{



    //wt_draw_dot(p.pos, 2, c);


    wt_r32 radius = 7;
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(p.pos.x - radius / 2, p.pos.y - radius / 2, 0.0f); //绘制这种纹理,pos在左下角
    glScalef(radius, radius, 0.0f);
    glBindTexture(GL_TEXTURE_2D, texture_ID_list[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f( 0.0,   0.0,   0.0);  // 纹理和四边形的左下
    glTexCoord2f(1.0, 0.0); glVertex3f( 1.0,   0.0,   0.0);  // 纹理和四边形的右下
    glTexCoord2f(1.0, 1.0); glVertex3f( 1.0,   1.0,   0.0);    // 纹理和四边形的右上
    glTexCoord2f(0.0, 1.0); glVertex3f( 0,     1.0,   0.0);    // 纹理和四边形的左上
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // wt_r32 radius = 10;
    // glEnable(GL_TEXTURE_2D);
    // glBindTexture(GL_TEXTURE_2D, texture_ID_list[0]);
    // glVertexPointer(2, GL_FLOAT, 0, wt_rect_data);
    // glTexCoordPointer(2, GL_FLOAT, 0, wt_rect_data);
    // glPushMatrix();
    // glTranslatef(p.pos.x - radius / 2, p.pos.y - radius / 2, 0.0f); //绘制这种纹理,pos在左下角
    // glScalef(radius, radius, 1.0f);
    // glDrawArrays(GL_TRIANGLE_STRIP, 0, wt_rect_count);
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
    //glDisable(GL_ALPHA_TEST);
    //glEnable(GL_BLEND);                         //启用混合
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glColor3f( 1.0f, 1.0f, 1.0f); //清除其他颜色，显示纹理本身颜色

}

void wt_end_draw()
{
    //glMatrixMode(GL_MODELVIEW);
    glutSwapBuffers();
}



void wt_draw(wt_world *w)
{

    wt_begin_draw();


    wt_draw_background(w->width);
    // wt_array *shapes = w->shapes;
    wt_draw_shapes(w->shapes);
    // glEnable(GL_ALPHA_TEST);
    // glAlphaFunc(GL_GEQUAL, 0.01);
    // glBindTexture(GL_TEXTURE_2D, texture_ID_list[0]);
    // wt_draw_fluid(w->fluid);
    // glDisable(GL_ALPHA_TEST);

    wt_draw_fluid_meta_ball(w->fluid);
    //wt_draw_fluid(w->fluid);

    wt_end_draw();
}


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


void wt_gl_init(GLvoid)
{
    glGenTextures(10, texture_ID_list);
    wt_gener_image_data();
    wt_load_bmp2("8.bmp", 5);
    texture_colorkey();
    wt_load_bmp2("back.bmp", 6);
    init_shader();
    glEnable(GL_BLEND);                         //启用混合
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //texture_colorkey();
    //glEnable(GL_TEXTURE_2D);
    // GLfloat values[2];
    // glGetFloatv(GL_LINE_WIDTH_GRANULARITY, values);
    // glGetFloatv(GL_LINE_WIDTH_RANGE, values);
    //glEnable(GL_POINT_SMOOTH);                          //点抗锯齿
    //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    //glEnable(GL_LINE_SMOOTH);                         //线抗锯齿
    //glEnable(GL_POLYGON_SMOOTH);                      //多边形抗锯齿
    //glEnableClientState(GL_VERTEX_ARRAY);               //开启画array功能
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    //glEnable(GL_ALPHA_TEST);
    //glAlphaFunc(GL_EQUAL, 0.7); //0是透明 大于是通过测试
    //glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);          //设置抗锯齿的参数
    //glClearColor(0.0, 0.0, 0.0, 1.0);                   //设置背景颜色

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
    //glTranslatef(0, 0, -15.0);
}


// void wt_gl_main(void (*wt_gl_key_fun)(unsigned char c, __attribute__((unused)) int x, __attribute__((unused))  int y), void (*wt_gl_display)())
// {
//     // glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
//     //    //glutInitWindowPosition(50, 50);
//     //    glutInitWindowPosition(350, 350);
//     //    glutInitWindowSize(400, 400);
//     //    glutCreateWindow("waterZ");
//     //    wt_gl_init ();//因为里面的抗锯齿,需要在创建窗口后调用才行

//     //    glutReshapeFunc(wt_gl_reshape);
//     //    glutDisplayFunc(wt_gl_display);
//     // //   glutKeyboardFunc(keyboard1);
//     //    glutIdleFunc(wt_gl_display);
//     //    glutMainLoop();
// }

#define GLEW_STATIC

#include <stdio.h>
#include "GL/glew.h"
#include "GL/glut.h"


GLuint   program_object;  // a handler to the GLSL program used to update
GLuint   vertex_shader;   // a handler to vertex shader. This is used internally
GLuint   fragment_shader; // a handler to fragment shader. This is used internally too

GLint loc_win_width;
GLint loc_world_width;
GLint loc_cir;
GLint loc_cir_num;
GLint win_width;

int mousex = 0;
int mousey = 0;


static const char *vertex_source =
{
    "void main(){"
        "gl_Position = gl_Vertex;"
    "}"
};

static const char *fragment_source =
{
    "\
    uniform float window_width;\
    uniform float world_width;\
    uniform int cir_num;\
    uniform vec3 cir[10];\
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
		if(threhold > 220 && threhold < 240){\
			gl_FragColor = vec4(242.0/255.0, 108.0/255.0, 45.0/255.0, 1.0);\
		}\
    }\
    "
};
// static const char *fragment_source =
// {
//   "\
//     uniform float window_width;\
//     uniform float world_width;\
//     uniform int cir_num;\
//     uniform vec3 cir[10];\
//     vec3 world_to_win_size(vec3 cir) {\
//      return cir / world_width * window_width;\
//     }\
//     float meta_fall_off(float dis_sq, float max_dis_sq) {\
//          if(dis_sq < max_dis_sq){\
//              float x = 1 - dis_sq / max_dis_sq;\
//              return (3.0f / 2.0f) * x * x;\
//          }else{\
//              return 0.0;\
//          }\
//     }\
//     void main(void)\
//     {\
//       for(int i = 0 ; i < cir_num ; i++){\
//           vec3 c = world_to_win_size(cir[i]);\
//           vec2 pos = gl_FragCoord.xy- c.xy;\
//           float dist_squared = dot(pos, pos);\
//           if(dist_squared < 400.0) {\
//             gl_FragColor = vec4(242.0/255.0, 108.0/255.0, 45.0/255.0, 1.0);\
//           }\
//       }\
//     }"
// };

int init(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);

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

    loc_win_width = glGetUniformLocation(program_object,"window_width");
    loc_world_width = glGetUniformLocation(program_object,"world_width");
    loc_cir = glGetUniformLocation(program_object,"cir");
    loc_cir_num = glGetUniformLocation(program_object,"cir_num");
    //glUseProgram(p);

    return 1;
}



GLfloat cir[10][3];
// Our rendering is done here
void render(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear Screen And Depth Buffer
    glLoadIdentity();                                   // Reset The Current Modelview Matrix

    //gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // bind the GLSL program
    // this means that from here the GLSL program attends all OpenGL operations
    glUseProgram(program_object);
    glUniform1f(loc_win_width,win_width);
    glUniform1f(loc_world_width,100);

    
    cir[0][0] = mousex;
    cir[0][1] = mousey;
    cir[0][2] = 10;
    cir[1][0] = 50;
    cir[1][1] = 50;
    cir[1][2] = 20;
    //printf("%f %f\n",cir[0],cir[1] );
    //glUniform1d(loc_cir_num,1);
    glUniform3fv(loc_cir,2,cir);
   	glUniform1i(loc_cir_num,2);

    // painting a quad
    glBegin(GL_QUADS);
    glVertex3f(-1, -1, 0.0);
    glVertex3f( 1,  -1, 0.0);
    glVertex3f( 1,   1, 0.0);
    glVertex3f(-1,  1, 0.0);
    glEnd();

    // unbind the GLSL program
    // this means that from here the OpenGL fixed functionality is used
    glUseProgram(0);


    // Swap The Buffers To Become Our Rendering Visible
    glutSwapBuffers();
}

// Our Reshaping Handler (Required Even In Fullscreen-Only Modes)
void reshape(int w, int h)
{
	win_width = w;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);     // Select The Projection Matrix
    glLoadIdentity();                // Reset The Projection Matrix
    //gluPerspective(45, (float)w / (float)h, 0.1, 100.0);
    gluOrtho2D(0, 100, 0, 100);
    glMatrixMode(GL_MODELVIEW);      // Select The Modelview Matrix
    glLoadIdentity();                // Reset The Modelview Matrix
}

// Our keyboard handler
void mouseMove(int x,int y){
    //
    float tmp_x = (float) x;
    float tmp_y = (float) y;
    tmp_x = (float)x / 500 * 100;
    tmp_y = (float)y / 500 * 100;
    mousex = tmp_x;
    mousey = 100 - tmp_y;
    //printf("%d %d\n",mousex,mousey );

}

// Main Function For Bringing It All Together.
int main(int argc, char **argv)
{
    glutInit(&argc, argv);                           // GLUT Initializtion
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);     // Display Mode (Rgb And Double Buffered)
    
    win_width = 500;
    glutInitWindowSize(win_width, win_width);
    glutCreateWindow("GLSL Hello World!");          // Window Title

    glewInit();
    if (glewIsSupported("GL_VERSION_2_0"))
        printf("Ready for OpenGL 2.0\n");
    else
    {
        printf("OpenGL 2.0 not supported\n");
        exit(1);
    }


    init();                                          // Our Initialization
    glutDisplayFunc(render);                         // Register The Display Function
    glutReshapeFunc(reshape);                        // Register The Reshape Handler
    //glutKeyboardFunc(keyboard);                      // Register The Keyboard Handler
    glutIdleFunc(render);                            // Do Rendering In Idle Time
    glutMotionFunc(mouseMove);
    glutPassiveMotionFunc(mouseMove);
    glutMainLoop();                                  // Go To GLUT Main Loop
    return 0;
}

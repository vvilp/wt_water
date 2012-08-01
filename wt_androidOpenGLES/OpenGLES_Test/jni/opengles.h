#ifndef _OPENGLES_H
#define _OPENGLES_H

#include <GLES/gl.h>
#include <GLES/glext.h>   
#include <stdlib.h>  
#include <math.h>  
#include <android/log.h>
#define  LOG_TAG    "OPENGELES_native"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)


void InitGL();
void SizeChanged(int width ,int height);
void RendererGL();

#endif


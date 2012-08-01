#include <jni.h>
#include "opengles.h"
#include "wt_main.h"

JNIEXPORT jint JNICALL Java_com_nativeFun_NativeFun_onSurfaceCreated(JNIEnv * env, jclass mObj){
	InitGL();
}

JNIEXPORT jint JNICALL Java_com_nativeFun_NativeFun_onSurfaceChanged(JNIEnv * env, jclass mObj, jint width, jint height){
	SizeChanged(width,height);  
}

JNIEXPORT jint JNICALL Java_com_nativeFun_NativeFun_onDrawFrame(JNIEnv * env, jclass mObj){
	RendererGL(); 
}

JNIEXPORT jint JNICALL Java_com_nativeFun_NativeFun_setGravity(JNIEnv * env, jclass mObj, jfloat x, jfloat y){
	wt_setGravity(x, y);
}
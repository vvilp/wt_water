#include <jni.h>
#include "wt_android_core.h"

//
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
	//wt_setGravity(x, y);
}

JNIEXPORT jint JNICALL Java_com_nativeFun_NativeFun_addFluidPartical(JNIEnv * env, jclass mObj, jfloat x, jfloat y){
	//wt_setGravity(x, y);
	wt_world_add_fluid(x , y);
}
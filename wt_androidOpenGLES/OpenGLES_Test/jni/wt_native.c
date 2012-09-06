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

//设置重力参数，最大是1
JNIEXPORT jint JNICALL Java_com_nativeFun_NativeFun_setFluidGravity(JNIEnv * env, jclass mObj, jfloat x, jfloat y){
	wt_vec g = wt_v(x,y);
	wt_world_set_fluid_gravity(g);
}

JNIEXPORT jint JNICALL Java_com_nativeFun_NativeFun_addFluidPartical(JNIEnv * env, jclass mObj, jfloat x, jfloat y){
	//wt_setGravity(x, y);
	wt_world_add_fluid(x , y);
}

JNIEXPORT jint JNICALL Java_com_nativeFun_NativeFun_setTexture
(JNIEnv * env, jclass mObj, jintArray texture)
{
 	GLuint *id = (GLuint *)(*env)->GetIntArrayElements(env, texture,0);
 	wt_set_texture_id(id);
 	//LOGI("*id : %d , index : %d ",*id , index);
}

JNIEXPORT jint JNICALL Java_com_nativeFun_NativeFun_setFluidDensity(JNIEnv * env, jclass mObj, jfloat density){
	//wt_setGravity(x, y);
	//wt_world_add_fluid(x , y);
	wt_world *w_world = wt_get_world();
	wt_world_set_pvf_density(w_world, density);
}

JNIEXPORT jint JNICALL Java_com_nativeFun_NativeFun_setFluidPlasticity(JNIEnv * env, jclass mObj, jfloat Plasticity){
	//wt_setGravity(x, y);
	//wt_world_add_fluid(x , y);
	wt_world *w_world = wt_get_world();
	wt_world_set_pvf_plastic(w_world, Plasticity);
}



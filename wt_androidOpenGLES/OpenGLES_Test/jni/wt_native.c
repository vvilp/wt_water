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

/*
 rigid body demo
*/
JNIEXPORT jint JNICALL Java_com_nativeFun_NativeFun_initRigidDemo1(JNIEnv * env, jclass mObj ){
	wt_reset_demo_index(1);
}

JNIEXPORT jint JNICALL Java_com_nativeFun_NativeFun_initRigidDemo2(JNIEnv * env, jclass mObj ){
	wt_reset_demo_index(2);
}

JNIEXPORT jint JNICALL Java_com_nativeFun_NativeFun_initRigidDemo3(JNIEnv * env, jclass mObj ){

}

/*
 soft body demo
*/
JNIEXPORT jint JNICALL Java_com_nativeFun_NativeFun_initSoftDemo1(JNIEnv * env, jclass mObj ){

}

JNIEXPORT jint JNICALL Java_com_nativeFun_NativeFun_initSoftDemo2(JNIEnv * env, jclass mObj ){

}

JNIEXPORT jint JNICALL Java_com_nativeFun_NativeFun_initSoftDemo3(JNIEnv * env, jclass mObj ){

}


/*
 Fluid body demo
*/
JNIEXPORT jint JNICALL Java_com_nativeFun_NativeFun_initFluidDemo1(JNIEnv * env, jclass mObj ){

}

JNIEXPORT jint JNICALL Java_com_nativeFun_NativeFun_initFluidDemo2(JNIEnv * env, jclass mObj ){

}

JNIEXPORT jint JNICALL Java_com_nativeFun_NativeFun_initFluidDemo3(JNIEnv * env, jclass mObj ){

}

JNIEXPORT jint JNICALL Java_com_nativeFun_NativeFun_addFluidPartical(JNIEnv * env, jclass mObj, jfloat x, jfloat y){
	//wt_setGravity(x, y);
	wt_world_add_fluid(x , y);
}

JNIEXPORT jint JNICALL Java_com_nativeFun_NativeFun_addRigidCir(JNIEnv * env, jclass mObj, jfloat x, jfloat y){
	wt_world *w_world = wt_get_world();
	wt_generate_circle(w_world,x,y,3.0);
}



//设置重力参数，最大是1
JNIEXPORT jint JNICALL Java_com_nativeFun_NativeFun_setFluidGravity(JNIEnv * env, jclass mObj, jfloat x, jfloat y){
	wt_vec g = wt_v(x,y);
	wt_world_set_fluid_gravity(g);
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



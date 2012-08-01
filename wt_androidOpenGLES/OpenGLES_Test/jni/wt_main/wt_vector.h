#ifndef WT_VECTOR_H
#define WT_VECTOR_H
#include "../wt_common/wt_common.h"
typedef struct {
	wt_r32 x;
	wt_r32 y;
} wt_vector,wt_vec;

static wt_status wt_v_is_0(wt_vec v){
	if(v.x==0 && v.y == 0){
		return 1;
	}
	return 0;
}

static wt_vec wt_v(wt_r32 x,wt_r32 y)
{
	wt_vec ret;
	ret.x = x; ret.y = y;
	return ret;
}

static wt_vec wt_vadd(wt_vec v1,wt_vec v2)
{
	wt_vec ret;
	ret.x = v1.x + v2.x; ret.y = v1.y + v2.y;
	return ret;
}

static wt_vec wt_vsub(wt_vec v1,wt_vec v2)
{
	wt_vec ret;
	ret.x = v1.x - v2.x; ret.y = v1.y - v2.y;
	return ret;
}

static wt_vec wt_vmuls(wt_vec v1,wt_r32 scalar)
{
	wt_vec ret;
	ret.x = v1.x * scalar; ret.y = v1.y * scalar;
	return ret;
}

static wt_r32 wt_vmulv(wt_vec v1,wt_vec v2)
{
	return v1.x*v2.y - v1.y*v2.x;
}

static wt_vec wt_vdiv(wt_vec v1,wt_r32 scalar)
{
	wt_vec ret;
	ret.x = v1.x / scalar; ret.y = v1.y / scalar;
	return ret;
}

static wt_r32 wt_vdot(wt_vec v1,wt_vec v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

//求向量的逆
static wt_vec wt_nvec(wt_vec v){
	return wt_v(-v.x,-v.y);
}

//计算 v1 在 v2 投影的向量 与v2方向无关 仅与v1有关
static wt_vec wt_vproj(wt_vec v1,wt_vec v2)
{
	wt_r32 res_dot = wt_vdot(v1, v2);
	wt_r32 t = res_dot / (v2.x*v2.x + v2.y*v2.y);
	return wt_vmuls(v2,t);
}

static wt_r32 wt_vlen(wt_vec v1)
{
	return wt_sqrt(v1.x*v1.x + v1.y*v1.y);
}

static wt_r32 wt_vlen2(wt_vec v1){
	return (v1.x*v1.x + v1.y*v1.y);
}

static wt_vec wt_vunit(wt_vec v){
	if(v.x == 0 && v.y == 0){
		wt_vec vv;
		vv.x = 0; vv.y = 0;
		return vv;
	}else{
		return wt_vmuls(v,1/wt_vlen(v));
	}
}

/// 计算垂直向量. (90 degree rotation)
static inline wt_vec wt_vperp(wt_vec v)
{
	return wt_v(-v.y, v.x);
}

/// 计算垂直向量. (-90 degree rotation)
static inline wt_vec wt_vnperp(wt_vec v)
{
	return wt_v(v.y, -v.x);
}

static wt_vec wt_VcrossS(wt_vec v,wt_r32 s)
{
	return wt_v(s * v.y, -s * v.x);
}

static wt_vec wt_ScrossV(wt_r32 s,wt_vec v)
{
	return wt_v(-s * v.y, s * v.x);
}
////计算向量的垂直单位向量
//static wt_vec wt_vertc_unitv(wt_vec v)
//{
//	wt_vec ver;
//	if(wt_cmp_real(v.y,0) == 0){
//		ver.x = 0;
//		ver.y = 1;
//	}else{
//		wt_r32 t = v.y * v.y / (v.x * v.x + v.y * v.y);
//		ver.x = wt_sqrt(t);
//		ver.y = -v.x/v.y*ver.x;
//	}
//	return ver;
//}

/*  向量逆时针旋转公式
 *  x1 = x0 * cosw - y0 * sinw
　　 y1 = x0 * sinw + y0 * cosw
 */
static wt_vec wt_vrotation(wt_vec v,wt_r32 w){
	wt_vec v1;
	wt_r32 cosw = cos(w);wt_r32 sinw = sin(w);

	v1.x = v.x * cosw - v.y * sinw;
	v1.y = v.x * sinw + v.y * cosw;
	return v1;
}

static void debug_vec(wt_vec v,char *s)
{
	printf("%s : %f --- %f\n",s,v.x,v.y);
}
#endif

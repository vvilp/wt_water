#ifndef WT_MATH_H
#define WT_MATH_H

#include <stdio.h>
#include <math.h>
#define WT_E            2.7182818284590452354
#define WT_LOG2E        1.4426950408889634074
#define WT_LOG10E       0.43429448190325182765
#define WT_LN2          0.69314718055994530942
#define WT_LN10         2.30258509299404568402
#define WT_PI           3.14159265358979323846
#define WT_PI_2         1.57079632679489661923
#define WT_PI_4         0.78539816339744830962
#define WT_1_PI         0.31830988618379067154
#define WT_2_PI         0.63661977236758134308
#define WT_2_SQRTPI     1.12837916709551257390
#define WT_SQRT2        1.41421356237309504880
#define WT_SQRT1_2      0.70710678118654752440

#define WT_MAX_R32 9999999999.0

typedef int wt_int32;
typedef short wt_int16;
typedef float wt_real32;
typedef double wt_real64;
typedef long double wt_real128;

typedef wt_int32 wt_i32;
typedef wt_int16 wt_i16;
typedef wt_real32 wt_r32;
typedef wt_real64 wt_r64;
typedef wt_real128 wt_r128;

//比较浮点数 0:相等 1:大于 -1:小于
static int wt_cmp_real(wt_r32 r1, wt_r32 r2)
{
    wt_r32 r = r1 - r2;
    //printf("r1 - r2 : %f\n", r1 - r2);
    if (r > -0.000001 && r < 0.000001){
        //printf(" == 0\n");
        return 0;
    }
    if (r > 0)
        return 1;
    if (r < 0)
        return -1;
}

static wt_r32 wt_fabs(wt_r32 r)
{
    return fabs(r);
}



//角度转弧度
static wt_r32 wt_atow(wt_r32 angular)
{
    return angular * WT_PI / 180;
}
//弧度转角度
static wt_r32 wt_wtoa(wt_r32 radian)
{
    return radian * 180 / WT_PI;
}

static wt_r32 wt_rmax(wt_r32 a, wt_r32 b)
{
    return a > b ? a : b;
}

static wt_r32 wt_rmin(wt_r32 a, wt_r32 b)
{
    return a > b ? b : a;
}

static wt_r32 wt_rclamp(wt_r32 a, wt_r32 min, wt_r32 max)
{
    return wt_rmin(wt_rmax(a, min), max);
}

static wt_r32 wt_rpow(wt_r32 a, wt_r32 p)
{
    return pow(a, p);
}

static inline wt_r32 wt_sqrt_inv_quick( wt_r32 number )
{
    long i;
    wt_r32 x2, y;
    const wt_r32 threehalfs = 1.5F;

    x2 = number * 0.5F;
    y   = number;
    i   = * ( long * ) &y;   // evil floating point bit level hacking
    i   = 0x5f3759df - ( i >> 1 ); // what the fuck?
    y   = * ( wt_r32 * ) &i;
    y   = y * ( threehalfs - ( x2 * y * y ) ); // 1st iteration
    // y   = y * ( threehalfs - ( x2 * y * y ) ); // 2nd iteration, this can be removed


    return y;
}

static wt_r32 wt_sqrt(wt_r32 r)
{
    return sqrt(r);
}

#endif

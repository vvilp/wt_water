#ifndef _WT_COMMON_H_INCLUDE
#define _WT_COMMON_H_INCLUDE

//some config with environment
#define WIN32 1
#define WT_DEBUG_ON 1
#define WT_INFO_ON 1

#if WIN32
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#endif


//============================
typedef int wt_status;
#define WT_OK 1
#define WT_ER 0

#include "wt_array.h"
#include "wt_math.h"
#include "wt_debug.h"
#include "wt_list.h"

static void wt_empty_fun(){}

#endif
#ifndef WT_ARRAY_H
#define WT_ARRAY_H
#include "wt_common.h"
typedef struct 
{
	/* data */
	int num;
	int max;
	void** array;
} wt_array;

wt_array* wt_array_init(int count);
wt_status wt_array_add(wt_array* array, void* obj);
wt_status wt_array_free(wt_array** array);
wt_status wt_array_clear(wt_array* array);
wt_status wt_array_remove(wt_array* array, void* obj);
#endif
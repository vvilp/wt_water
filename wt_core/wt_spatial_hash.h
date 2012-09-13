#ifndef WT_SPATIAL_HASH
#define WT_SPATIAL_HASH

#include "wt_core.h"

const static int prime1 = 11447;//73856093;
const static int prime2 = 9133;//19349663;
const static int prime3 = 83492791;

typedef struct 
{
	wt_array *hash;
	wt_i32 max_num;
	wt_r32 cell_size;
} wt_spatial_hash;

wt_spatial_hash* wt_init_spatial_hash(wt_i32 max_num, wt_r32 cell_size);

void wt_add_spatial_hash(wt_vec pos_tl, wt_vec pos_br, void *obj);

void wt_clear_spatial_hash();

#endif
#include "wt_core.h"

wt_spatial_hash *wt_init_spatial_hash(wt_i32 max_num, wt_r32 cell_size)
{
    wt_spatial_hash *hash = (wt_spatial_hash *)malloc(sizeof(wt_spatial_hash));
    hash->hash = wt_array_init(max_num);
    hash->max_num = max_num;
    hash->cell_size = cell_size;
    return hash;
}

wt_i32 wt_get_spatial_hash_key(int x, int y, int max)
{
    return ((x * prime1) ^ (y * prime2)) % max;
}

//根据 AABB 加入 hash 将obj指针加入hash
//pos_tl: pos at left-top   pos_br: pos at right-bottom
void wt_add_to_spatial_hash(wt_spatial_hash *hash, wt_vec pos_tl, wt_vec pos_br, void *obj)
{
	wt_debug("wt_add_to_spatial_hash \n",1);
    int cellx1 = floor(pos_tl.x / hash->cell_size);
    int cellx2 = floor(pos_br.x / hash->cell_size);
    int celly2 = floor(pos_tl.y / hash->cell_size);
    int celly1 = floor(pos_br.y / hash->cell_size);

	wt_debug("%d %d %d %d \n",cellx1,cellx2,celly1,celly2);
    for (int i = cellx1 ; i <= cellx2 ; i++)
    {
        for (int j = celly1; j <= celly2; j++)
        {
            wt_array *hash_table = hash->hash;
            wt_i32 key = wt_get_spatial_hash_key(i, j, hash->max_num);
            wt_debug("key: %d %d : %d \n", i, j, key);
            system("pause");
            wt_array *list = hash_table->array[key];
            if (list == 0)
            {
                //hash表中 建立可变列
                wt_debug("初始化列表 \n",1);
                system("pause");
                list = wt_array_init(5);
            }
            wt_array_add(list, obj);
        }
    }

}

void wt_clear_spatial_hash()
{

}
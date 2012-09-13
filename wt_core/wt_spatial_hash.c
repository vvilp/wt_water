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
    //wt_debug("x * prime1 %d \n",x * prime1);
    //wt_debug("(y * prime2) %d \n",(y * prime2));
    //wt_debug("(x * prime1) ^ (y * prime2) %d \n",(x * prime1) ^ (y * prime2));

    return ((x * prime1) ^ (y * prime2)) % max;
}

//根据 AABB 加入 hash 将obj指针加入hash
//pos_tl: pos at left-top   pos_br: pos at right-bottom
void wt_add_to_spatial_hash(wt_spatial_hash *hash, wt_vec pos_tl, wt_vec pos_br, void *obj)
{
	//wt_debug("wt_add_to_spatial_hash \n",1);
    int cellx1 = floor(pos_tl.x / hash->cell_size);
    int cellx2 = floor(pos_br.x / hash->cell_size);
    int celly2 = floor(pos_tl.y / hash->cell_size);
    int celly1 = floor(pos_br.y / hash->cell_size);

    cellx1 = cellx1 < 0 ? 0 : cellx1;
    cellx2 = cellx2 < 0 ? 0 : cellx2;
    celly1 = celly1 < 0 ? 0 : celly1;
    celly2 = celly2 < 0 ? 0 : celly2;

	//wt_debug("%d %d %d %d \n",cellx1,cellx2,celly1,celly2);
    for (int i = cellx1 ; i <= cellx2 ; i++)
    {
        for (int j = celly1; j <= celly2; j++)
        {
            wt_array *hash_table = hash->hash;
            wt_i32 key = wt_get_spatial_hash_key(i, j, hash->max_num);
            //wt_debug("key: %d %d : %d \n", i, j, key);
            //system("pause");
            wt_array *list = hash_table->array[key];
            if (list == 0)
            {
                //hash表中 建立可变列
                //wt_debug("init hash \n",1);
                //system("pause");
                list = wt_array_init(10);
                hash_table->array[key] = list;
            }
           //wt_debug("add to hash \n",1);
            wt_array_add(list, obj);
            //wt_debug("add to hash ok\n",1);
        }
    }

}

void wt_remove_from_spatial_hash(wt_spatial_hash *hash, wt_vec pos_tl, wt_vec pos_br, void *obj)
{
    int cellx1 = floor(pos_tl.x / hash->cell_size);
    int cellx2 = floor(pos_br.x / hash->cell_size);
    int celly2 = floor(pos_tl.y / hash->cell_size);
    int celly1 = floor(pos_br.y / hash->cell_size);

    cellx1 = cellx1 < 0 ? 0 : cellx1;
    cellx2 = cellx2 < 0 ? 0 : cellx2;
    celly1 = celly1 < 0 ? 0 : celly1;
    celly2 = celly2 < 0 ? 0 : celly2;

    for (int i = cellx1 ; i <= cellx2 ; i++)
    {
        for (int j = celly1; j <= celly2; j++)
        {
            wt_array *hash_table = hash->hash;
            wt_i32 key = wt_get_spatial_hash_key(i, j, hash->max_num);
            wt_array *list = hash_table->array[key];
            if (list != 0)
            {
                wt_array_remove(list,obj);
            }
        }
    }
}

wt_array *wt_get_neighbour_hash_list(wt_spatial_hash *hash,int x,int y)
{
    wt_array *hash_list = hash->hash;
    wt_i32 key = wt_get_spatial_hash_key(x, y, hash->max_num);
    return hash_list->array[key];
}


void wt_clear_spatial_hash(wt_spatial_hash *hash)
{
    for(int i = 0 ; i < hash->max_num ; i++){
        wt_array *list = hash->hash->array[i];
        if(list != NULL){
            wt_array_clear(list);
        }
    }
}
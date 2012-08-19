#ifndef WT_SPATIAL_TABLE_H
#define WT_SPATIAL_TABLE_H

#include "wt_core.h"

typedef struct
{
    wt_array *all_list;
    void ** *table; //二维数组存指针

    wt_r32 cell_size;
    int cell_num;
    wt_r32 world_size;

    wt_array *near_list;
} wt_spatial_table;

typedef struct {
    wt_i32 x_small;
    wt_i32 x_large;
    wt_i32 y_small;
    wt_i32 y_large;
} wt_spatial_box;


static wt_spatial_box wt_get_spatial_box(wt_r32 world_size,wt_r32 cell_size, wt_r32 x, wt_r32 y, wt_r32 r)
{
    int cellx1 = floor((x - r) / cell_size);
    int cellx2 = floor((x + r) / cell_size);
    int celly1 = floor((y - r) / cell_size);
    int celly2 = floor((y + r) / cell_size);
    cellx1 = cellx1 < 0 ? 0 : cellx1;
    celly1 = celly1 < 0 ? 0 : celly1;
    cellx2 = cellx2 >= world_size ? world_size -1 : cellx2;
    celly2 = celly2 >= world_size ? world_size -1 : celly2;
    wt_spatial_box res ;
    res.x_small = cellx1;
    res.y_small = celly1;
    res.x_large = cellx2;
    res.y_large = celly2;
    return res;
}

static wt_spatial_table *wt_create_spatial_table(wt_r32 world_size, wt_r32 cell_size)
{
    int num_cell = (int)(world_size / cell_size) + 1;
    wt_spatial_table *table = (wt_spatial_table *)malloc(sizeof(wt_spatial_table));
    table->all_list = wt_array_init(100);
    table->near_list = wt_array_init(100);
    table->cell_size = cell_size;
    table->cell_num = num_cell;
    table->world_size = world_size;
    void ***t;
    //num_cell +=10;
    t =  calloc (num_cell, sizeof(void **));
    for (int i = 0 ; i < num_cell ; i++)
    {
        t[i] = calloc(num_cell, sizeof(void *));
    }
    for (int i = 0 ; i < num_cell ; i++)
    {
        for (int j = 0 ; j < num_cell ; j++)
        {

            wt_list * list = wt_list_init();
            t[i][j] = list;
            //wt_debug("init a list \n", 1);
        }
    }

    table->table = t;


    return table;
}


static void wt_spatial_table_add_obj(wt_spatial_table *table, void *obj, wt_r32 x, wt_r32 y, wt_r32 r, int is_add_to_alllist)
{
    if(is_add_to_alllist){
        wt_array_add(table->all_list, obj);
    }

    wt_spatial_box box = wt_get_spatial_box(table->world_size, table->cell_size, x, y, r);

    for (int i = box.x_small ; i <= box.x_large ; i++)
    {
        for (int j = box.y_small ; j <= box.y_large ; j++)
        {
            //wt_debug("wt_spatial_table_add_obj %d, %d\n",i,j);
            // wt_array *list =  table->table[i][j];
            // if (list == NULL)
            // {
                
            //     //wt_debug("list == NULL create \n", 1);
            //     list = wt_array_init(50);
                
            //     table->table[i][j] = list;
            // }
            // wt_array_add(list, obj);
            wt_list *list = table->table[i][j];
            //wt_list_part *p = wt_list_part_create(obj);
            wt_list_push(list, obj);
            //wt_debug("add to list \n", 1);
        }
    }

}

static void wt_spatial_table_get_near_list(wt_spatial_table *table, void *obj, wt_r32 x, wt_r32 y, wt_r32 r)
{
    wt_array *near_list = table->near_list;
    wt_array_clear(near_list);

    wt_spatial_box box = wt_get_spatial_box(table->world_size, table->cell_size, x, y, r);
    for (int i = box.x_small ; i <= box.x_large ; i++)
    {
        for (int j = box.y_small ; j <= box.y_large ; j++)
        {
            // wt_array *list =  table->table[i][j];

            // if (list != NULL)
            // {
            //     //wt_debug("list != NULL \n", 1);
            //     for (int k = 0 ; k < list->num ; k++)
            //     {
            //         if (list->array[k] != NULL && list->array[k] != obj)
            //         {
            //             wt_array_add(near_list, list->array[k]);
            //         }
            //     }
            // }

            wt_list * list = table->table[i][j];
            wt_list_part *p = list->first;
            while(p!=NULL) {
                if(p->obj != obj) {
                    wt_array_add(near_list, p->obj);
                }
                p = p->next;
            }
        }
    }
}

static void wt_spatial_table_update_obj(wt_spatial_table *table, void *obj, wt_vec pos_now, wt_vec pos_pre, wt_r32 r)
{
    wt_spatial_box box_now = wt_get_spatial_box(table->world_size, table->cell_size, pos_now.x, pos_now.y, r);
    wt_spatial_box box_pre = wt_get_spatial_box(table->world_size, table->cell_size, pos_pre.x, pos_pre.y, r);

    if(box_now.x_large == box_pre.x_large &&
       box_now.y_large == box_pre.y_large && 
       box_now.x_small == box_pre.x_small &&
       box_now.y_small == box_pre.y_small )
    {
        //wt_debug(" position not change \n", 1);
        return;
    }

    for (int i = box_pre.x_small ; i <= box_pre.x_large ; i++)
    {
        for (int j = box_pre.y_small ; j <= box_pre.y_large ; j++)
        {
            //wt_debug("del : %d %d\n", i,j);
            wt_list_del(table->table[i][j], obj);
        }
    }

    for (int i = box_now.x_small ; i <= box_now.x_large ; i++)
    {
        for (int j = box_now.y_small ; j <= box_now.y_large ; j++)
        {
            wt_list_push(table->table[i][j], obj);
        }
    }

}

#endif
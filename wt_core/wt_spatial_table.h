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
            t[i][j] = NULL;
        }
    }

    table->table = t;
    //if(table->table[2][2] == NULL) wt_debug("NULL\n",1);
    //table
    return table;
}

static void wt_spatial_table_add_obj_table_only(wt_spatial_table *table, void *obj, wt_r32 x, wt_r32 y, wt_r32 r)
{
    //wt_array_add(table->all_list, obj);
    wt_r32 cell_size = table->cell_size;
    //table->all_list
    //wt_r32 world_size = table->
    int cellx1 = floor((x - r) / cell_size);
    int cellx2 = floor((x + r) / cell_size);
    int celly1 = floor((y - r) / cell_size);
    int celly2 = floor((y + r) / cell_size);
    cellx1 = cellx1 < 0 ? 0 : cellx1;
    celly1 = celly1 < 0 ? 0 : celly1;
    cellx2 = cellx2 >= table->world_size ? table->world_size -1 : cellx2;
    celly2 = celly2 >= table->world_size ? table->world_size -1: celly2;

    //wt_debug("cellx %d %d \n", cellx1, cellx2);
    //wt_debug("celly %d %d \n", celly1, celly2);
    for (int i = cellx1 ; i <= cellx2 ; i++)
    {
        for (int j = celly1 ; j <= celly2 ; j++)
        {
            //wt_debug("wt_spatial_table_add_obj_table_only %d, %d\n",i,j);
            //wt_debug("wt_array_init(5) %d, %d\n",i,j);
            wt_array *list =  table->table[i][j];
            if (list == NULL) {
            	list = wt_array_init(50);
            	//wt_debug("wt_array_init(5) %d, %d\n",i,j);
            	table->table[i][j] = list;
            }
            	
            wt_array_add(list, obj);

        }
    }

}

static void wt_spatial_table_add_obj(wt_spatial_table *table, void *obj, wt_r32 x, wt_r32 y, wt_r32 r)
{
    wt_array_add(table->all_list, obj);
    wt_r32 cell_size = table->cell_size;
    //table->all_list
    //wt_r32 world_size = table->
    int cellx1 = floor((x - r) / cell_size);
    int cellx2 = floor((x + r) / cell_size);
    int celly1 = floor((y - r) / cell_size);
    int celly2 = floor((y + r) / cell_size);
    cellx1 = cellx1 < 0 ? 0 : cellx1;
    celly1 = celly1 < 0 ? 0 : celly1;
    cellx2 = cellx2 >= table->world_size ? table->world_size -1 : cellx2;
    celly2 = celly2 >= table->world_size ? table->world_size -1: celly2;

    for (int i = cellx1 ; i <= cellx2 ; i++)
    {
        for (int j = celly1 ; j <= celly2 ; j++)
        {
            //wt_debug("wt_spatial_table_add_obj %d, %d\n",i,j);
            wt_array *list =  table->table[i][j];
            if (list == NULL)
            {
                
                //wt_debug("list == NULL create \n", 1);
                list = wt_array_init(50);
                
                table->table[i][j] = list;
            }
            wt_array_add(list, obj);
        }
    }

}

static void wt_spatial_table_get_near_list(wt_spatial_table *table, void *obj, wt_r32 x, wt_r32 y, wt_r32 r)
{
    wt_array *near_list = table->near_list;
    wt_array_clear(near_list);
    wt_r32 cell_size = table->cell_size;
    int cellx1 = floor((x - r) / cell_size);
    int cellx2 = floor((x + r) / cell_size);
    int celly1 = floor((y - r) / cell_size);
    int celly2 = floor((y + r) / cell_size);
    cellx1 = cellx1 < 0 ? 0 : cellx1;
    celly1 = celly1 < 0 ? 0 : celly1;
    cellx2 = cellx2 >= table->world_size ? table->world_size-1 : cellx2;
    celly2 = celly2 >= table->world_size ? table->world_size-1 : celly2;
    for (int i = cellx1 ; i <= cellx2 ; i++)
    {
        for (int j = celly1 ; j <= celly2 ; j++)
        {

            
            wt_array *list =  table->table[i][j];

            if (list != NULL)
            {
                //wt_debug("list != NULL \n", 1);
                for (int k = 0 ; k < list->num ; k++)
                {
                    if (list->array[k] != NULL && list->array[k] != obj)
                    {
                        wt_array_add(near_list, list->array[k]);
                    }
                }
            }
        }
    }
}

#endif
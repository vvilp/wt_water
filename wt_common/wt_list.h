#ifndef _WT_LIST_H_
#define _WT_LIST_H_
#include "wt_common.h"


typedef  struct 
{
	void *obj;
	struct wt_list_part *next;
} wt_list_part;

typedef struct 
{
	wt_list_part * first;
	//wt_list_part * last;
	wt_i32 num;

} wt_list;


wt_list* wt_list_init();
void wt_list_push(wt_list* l, void *obj);
void wt_list_pop(wt_list* l);
void wt_list_del(wt_list* l, void *obj);
wt_list_part * wt_list_part_create(void *p);
void wt_list_clear(wt_list* l);

#endif
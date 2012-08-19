#include "wt_common.h"

wt_list* wt_list_init()
{
	wt_list* l = (wt_list *) malloc (sizeof(wt_list));
	l->num = 0;
	l->first = NULL;
	//l->first->next = NULL;
	//l->last = NULL;
	//l->last->next = NULL;
	return l;
}

wt_list_part * wt_list_part_create(void *p)
{
	wt_list_part* list_part = (wt_list_part *) malloc (sizeof(wt_list_part));
	list_part->obj = p;
	list_part->next = NULL;
	return list_part;
}

void wt_list_push(wt_list* l, void *obj)
{
	wt_list_part *p = wt_list_part_create(obj);
	if(l->num == 0) {
		l->first = p;
		l->first->next = NULL;
		//l->last = p;
		//l->last->next = NULL;
	}else{
		// l->last->next = p;
		// l->last = p;
		// l->last->next = NULL;
		p->next = l->first;
		l->first = p;
	}

	l->num++;
}
void wt_list_pop(wt_list* l)
{

}

void wt_list_del(wt_list* l, void *obj)
{
	wt_list_part *p_pre = NULL;
	wt_list_part *p_now = l->first;
	int is_find = 0;
	while(p_now != NULL){
		if(p_now->obj == obj) {
			is_find = 1;
			//wt_debug("find del \n", 1);
			break;
		}
		p_pre = p_now;
		p_now = p_now->next; 
	}

	if(is_find == 1) {
		//wt_debug("wt_list_del\n", 1);
		if(p_pre == NULL) {

			l->first = p_now->next;
			//l->last = NULL;
		}else{
			p_pre->next = p_now->next;
		}
		l->num--;
		free(p_now);
		p_now = NULL;
	}

}

void wt_list_clear(wt_list* l)
{
	if(l->num == 0) return;
	wt_list_part *p_now = l->first;
	wt_list_part *p_pre = NULL;
	while(p_now != NULL){
		if(p_pre != NULL) {
			free(p_pre);
			p_pre = NULL;
		}
		p_pre = p_now;
		p_now = p_now->next;
	}
	free(p_pre);
	p_pre= NULL;
	l->first = NULL;
	l->num=0;
}
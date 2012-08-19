#include "wt_common.h"

int main()
{
    //wt_info("asd: %f",0.1);
    wt_list *list = wt_list_init();
    printf("NUM: %d \n", list->num);
    system("pause");

    for (int i = 0 ; i < 1000000 ; i++)
    {
        //int a = 1;
        //int *al = &a;
        wt_list_part *p = wt_list_part_create(NULL);
        wt_list_push(list, p);

    }
    printf("NUM: %d \n", list->num);
    system("pause");

    wt_list_clear(list);

    printf("NUM: %d \n", list->num);
    system("pause");



    // int a = 1;
    // int b = 2;
    // int c = 3;
    // int *al = &a;
    // int *bl = &b;
    // int *cl = &c;

    // wt_list_part *p1, *p2, *p3;

    // wt_list_part *p = wt_list_part_create(al);
    // wt_list_push(list, p);
    // p1 = p;

    // p = wt_list_part_create(bl);
    // wt_list_push(list, p);
    // p2 = p;

    // p = wt_list_part_create(cl);
    // wt_list_push(list, p);
    // p3 = p;

    // p = list->first;
    // while ( p != NULL )
    // {
    //     printf("%d\n", *((int *)p->obj));
    //     p = p->next;
    // }

    // printf("\n");


    // printf("%d\n", p3 == NULL);
    // wt_list_del(list, p3);
    // printf("%d\n", p3 == NULL);
    // //wt_list_del(list, p3);

    // p = list->first;
    // while ( p != NULL )
    // {
    //     printf("%d\n", *((int *)p->obj));
    //     p = p->next;
    // }

}
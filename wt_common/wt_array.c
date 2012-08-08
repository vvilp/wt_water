#include "wt_common.h"

int wt_array_each_count = 4;
wt_array* wt_array_init(int count)
{
	wt_array* array = (wt_array*) calloc(1, sizeof(wt_array));
	(array) -> num = 0;
	(array) -> max = count;
	(array) -> array = calloc(count, sizeof(void**));
	memset((array) -> array,0,count*sizeof(void**));
	wt_array_each_count = count;
	return array;
}

wt_status wt_array_add(wt_array* array, void* obj)
{
	//printf("array -> num : %d\n",array -> num);
	if (array->num == array -> max)
	{
		//wt_debug("array add\n",1);
		array -> max += wt_array_each_count;
		array -> array = realloc(array -> array, sizeof(void**) * array -> max);
	}
	array -> array[array -> num++] = obj;

	//printf("array -> num : %d\n",array -> num);
}

wt_status wt_array_remove(wt_array* array, void* obj)
{
	int index =-1;
	for(int i = 0 ; i < array->num ; i++){
		if(array->array[i] == obj){
			index = i;
		}
		if(i > index && index != -1){
			array->array[i-1] = array->array[i]; 
		}
	}
	if(index!=-1){
		array->num--;
	}
}

wt_status wt_array_free(wt_array** array)
{

	if (*array != NULL)
	{
		free((*array) -> array);
		free(*array);
		*array = NULL;
	}
}

//清空 并释放所存指针
wt_status wt_array_clear(wt_array* array)
{
	if (array != NULL)
	{
		array->num = 0;
	}

}



//====
#if 0

typedef struct{
	int a;
} test;

void free_test_array(wt_array *array){
	for(int i=0;i<array->num;i++){
		test* t = array->array[i];
		free(t);
	}
	wt_array_free(&array);
}
int main(int argc, char const *argv[])
{
	int a;
	wt_array *array = wt_array_init(100000);

	for(int i = 0 ; i < 100000 ; i++){
		test* t = malloc (sizeof(test));
		wt_array_add(array,t);
	}

	scanf("%d",&a);
	free_test_array(array);
	scanf("%d",&a);
	//printf("free:\n");
	//system("pause");
	//wt_array_free(&array);
	//printf("freed:\n");

	//test* t = (test*) malloc (sizeof(test) * 1000000);
	//memset(t,0,sizeof(test) * 1000000);
	//scanf("%d",&a);
	//free(t);
	//scanf("%d",&a);
}
#endif

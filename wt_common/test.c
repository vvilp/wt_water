#include "wt_common.h"

int main()
{
	//wt_info("asd: %f",0.1);
	for(int i = 0 ; i < 100 ; i++ ){
    wt_r32 a = (wt_r32)i;
	wt_r32 b = wt_sqrt_inv_quick(a);
	wt_r32 c = wt_sqrt(a);

	wt_debug("a:%f | inv sqrt | b:%f, c:%f \n",a, b,1.0/c);
	}


}
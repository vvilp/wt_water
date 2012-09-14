CC     = gcc 
CFLAGS =  -w -O -g -fopenmp -std=c99 
LINKS  = -lm -lglut -lGL -lGLU  

FILES =  \
../../wt_core/wt_body.c \
../../wt_core/wt_shape.c \
../../wt_core/wt_world.c  \
../../wt_common/wt_array.c \
../../wt_core/wt_collision.c \
../glew.c \
../wt_gl.c \
../../wt_core/wt_pvf_fluid.c \
../../wt_core/wt_spatial_hash.c \
../test.c 
#../../wt_core/wt_partical.c \
		
			
main : $(FILES)
	$(CC)  -o test  $(FILES) $(CFLAGS) $(LINKS)

#test.o : ../test.c 
#	$(CC) $(CFLAGS) -c  ../test.c 	

CC     = gcc 
CFLAGS =  -w -O -g -std=c99 
LINKS  = -lopengl32 -L../Dll   -lglu32 -lglut32 -lglaux

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
#../../wt_core/wt_partical.c \
		
			
main : test.o $(FILES)
	$(CC)  -o test test.o $(FILES) $(CFLAGS) $(LINKS)

test.o : ../test.c 
	$(CC) $(CFLAGS) -c  ../test.c 	

#wt_array.o : ../../wt_common/wt_array.c
#	$(CC) $(CFLAGS) -c  ../../wt_common/wt_array.c
#wt_world.o : ../wt_world.c 
#	$(CC) $(CFLAGS) -c  ../wt_world.c 
#wt_body.o : ../wt_body.c 
#	$(CC) $(CFLAGS) -c  ../wt_body.c 	
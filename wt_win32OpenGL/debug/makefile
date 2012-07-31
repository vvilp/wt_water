CC     = gcc 
CFLAGS =  -O -g -std=c99 
LINKS  = -L.  -lopengl32 -lglu32 -lglut32

FILES =  \
../../wt_core/wt_body.c \
../../wt_core/wt_shape.c \
../../wt_core/wt_world.c  \
../../wt_common/wt_array.c \
../../wt_core/wt_collision.c \
../wt_gl.c \
		
			
main : test.o $(FILES)
	$(CC) $(CFLAGS) $(LINKS) -o test test.o $(FILES)

test.o : ../test.c 
	$(CC) $(CFLAGS) -c  ../test.c 	

#wt_array.o : ../../wt_common/wt_array.c
#	$(CC) $(CFLAGS) -c  ../../wt_common/wt_array.c
#wt_world.o : ../wt_world.c 
#	$(CC) $(CFLAGS) -c  ../wt_world.c 
#wt_body.o : ../wt_body.c 
#	$(CC) $(CFLAGS) -c  ../wt_body.c 	
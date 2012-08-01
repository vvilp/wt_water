CC     = gcc 
CFLAGS =  -O -g -std=c99 
LINKS  = -L.  -lopengl32 -lglu32 -lglut32

FILES = ../../wt_common/wt_array.c ../wt_world.c  ../wt_body.c 	../wt_gl.c 	
		
			
main : main.o $(FILES)
	$(CC) $(CFLAGS) $(LINKS) -o main main.o $(FILES)

main.o : ../main.c 
	$(CC) $(CFLAGS) -c  ../main.c 	

#wt_array.o : ../../wt_common/wt_array.c
#	$(CC) $(CFLAGS) -c  ../../wt_common/wt_array.c
#wt_world.o : ../wt_world.c 
#	$(CC) $(CFLAGS) -c  ../wt_world.c 
#wt_body.o : ../wt_body.c 
#	$(CC) $(CFLAGS) -c  ../wt_body.c 	
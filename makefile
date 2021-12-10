OBJ = main.o fileio.o raymath.o raytracer.o  

APPNAME = raytracer		

$(APPNAME) : $(OBJ)	

	gcc -o $(APPNAME) $(OBJ) -lm -lpthread

%.o: %.c	

	gcc -g -c -o $@ $<  

clean:
	rm $(OBJ) $(APPNAME)

#makefile syntax
#target: dependencies
#action 
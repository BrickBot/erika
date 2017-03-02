export CPP = g++
export CPP_OPTS = -fexceptions -Wall -O2 -D_EXC_ -D_CHECKS_ -D_DEBUG_
export LD = g++
export OBJS = namedobject.o provaControl3.o
export SRC = $(patsubst %.o, %.cpp, $(OBJS))

.PHONY : prova depend clean

prova : $(OBJS) makefile _.dep
	$(LD) $(OBJS) -o prova

depend :
	$(CPP) -MM $(CPP_OPTS) $(SRC) > _.dep

_.dep :
	$(CPP) -MM $(CPP_OPTS) $(SRC) > _.dep


%.o : %.cpp makefile 
	$(CPP) -c  $(CPP_OPTS) $< -o  $@
clean:
	rm *.o
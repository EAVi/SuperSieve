#Makefile for the Supersieve function

#executable name
EXEC=ssieve
#source files
SRCS=main.cpp scale.cpp sieve.cpp
#object files
OBJS=$(SRCS:.cpp=.o)

$(EXEC): $(OBJS)
	mpic++ -o $@ $(OBJS)

$(OBJS):
	mpic++ -c $*.cpp

clean:
	rm $(OBJS)

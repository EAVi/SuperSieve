#Makefile for the Supersieve function

#executable name
EXEC=ssieve
#source files
OMPISRCS=binarray.cpp main.cpp scale.cpp sieve.cpp
CUDASRCS=cuda_sieve.cu
#object files
OMPIOBJS=$(OMPISRCS:.cpp=.o)
CUDAOBJS=$(CUDASRCS:.cu=.o)

OBJS=$(OMPIOBJS)
OBJS+=$(CUDAOBJS)

$(EXEC): $(OMPIOBJS) $(CUDAOBJS)
	mpic++ -o $@ $(OBJS) -lcudart -L /usr/local/cuda/lib64 

$(OMPIOBJS):
	mpic++ -c $*.cpp

$(CUDAOBJS):
	nvcc -ccbin /usr/bin/gcc53 -c $*.cu

clean:
	rm $(OMPIOBJS) $(CUDAOBJS)

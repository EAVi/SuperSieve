#include <cassert>
#include <iostream>
#include "scale.h"
#include "sieve.h"
#include "mpi.h"


using namespace std;

int main(int argc, char* args[])
{
	int nsize = 100;
	if (argc == 2)
	{
		nsize = atoi(args[1]);
	}
	int* list = NULL;
	int listsize = 0;
	
	//process numbers and stuff
	int np = 1;
	int rank = -1;
	double begin, end, diff;

	MPI_Status status;
	MPI_Init(&argc, &args);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &np);

	begin = MPI_Wtime();
	Sieve sieve(nsize, np, rank);
	sieve.findPrimes();

	//timing stuff
	MPI_Barrier(MPI_COMM_WORLD);
	end = MPI_Wtime();
	diff = end - begin;

	if (rank == 0)
	{
		sieve.getList(&list, &listsize);
	 //list primes
	/*
		for(int i = 0; i < listsize; ++i)
			printf("%i, ", list[i]);
	*/
	//state list size
		printf("there exists %i primes up to %i\n", listsize, nsize);	
		printf("execution time: %f seconds\n", diff);
	}
	printf("diff = %f, lat = %f, %f% \n", sieve.getTiming(), sieve.getLatency(), sieve.getLatency() / (sieve.getLatency() + sieve.getTiming()));

	sieve.clean();
	MPI_Finalize();
	return 0;
}

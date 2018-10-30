#include <cassert>
#include <iostream>
#include "scale.h"
#include "sieve.h"
#include "mpi.h"


using namespace std;

int main(int argc, char* args[])
{
	int nsize = 100;
	int cudasize = -1;
	if (argc >= 2 )
	{
		nsize = atoi(args[1]);
	}
	if (argc == 3)
	{
		cudasize =  atoi(args[2]);
	}
	
	int* list = NULL;
	int listsize = 0;
	
	//process numbers and stuff
	int np = 1;
	int rank = -1;
	double begin, end, diff;
	//process name
	int pnamelen = 0;
	char pname[MPI_MAX_PROCESSOR_NAME];

	MPI_Status status;
	MPI_Init(&argc, &args);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &np);
	MPI_Get_processor_name(pname, &pnamelen);

	begin = MPI_Wtime();
	Sieve sieve(nsize, np, rank, cudasize);
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
	MPI_Barrier(MPI_COMM_WORLD);

	double pdiff = sieve.getTiming();//amount of time used on calculation
	double lat = sieve.getLatency();//communication latency(through MPI_Allreduce)
	double perlat = 100*lat/(pdiff + lat);//percent of program in communication latency
	
	//use barriers to print outputs in order
	for (int i = 0; i < np; ++i)
	{
		if (rank == i)
			printf("rank = %i, pname = %s, diff = %f, lat = %f, %f% \n", rank, pname, pdiff, lat, perlat);
		MPI_Barrier(MPI_COMM_WORLD);
	}

	sieve.clean();
	MPI_Finalize();
	return 0;
}

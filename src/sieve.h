#ifndef SIEVE_H
#define SIEVE_H

#include <cstdlib>
#include <cstdio>
#include "mpi.h"
#include "scale.h"
#include "binarray.h"

class Sieve
{
public:
	Sieve(int n, int np, int rank, int cudasize);
	void findPrimes();
	void getList(int** list, int* listsize);
	void clean();

	double getTiming();
	double getLatency();
private:
	//MPI constants
	const int mMaxPrime;
	const int mProcessSize;
	const int mProcessRank;
	const int mUseCUDA;//use cuda when problem reaches given size, always use if 0, never use if -1
	
	int* mList;
	int mListSize;
	BinArray mArgArr;
	int mArgArrSize;
	char* mCUDA_binarray;
	int mHighestPrime;
	
	void mSieve();
	bool mKernel();//the iterative and complex part of the algorithm
	void mReduce();
	void mInitialize();
	bool mAllocate();
	void mDeallocate();

	//error
	bool mError;
	//latency and timing
	double mDiff;
	double mLatency;
};

#endif

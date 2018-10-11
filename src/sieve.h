#ifndef SIEVE_H
#define SIEVE_H

#include <cstdlib>
#include <cstdio>
#include "mpi.h"
#include "scale.h"

const char kPrime = 0;
const char kConsonant = 1;
const char kNonPrime = 1;


class Sieve
{
public:
	Sieve(int n, int np, int rank);
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
	
	int* mList;
	int mListSize;
	char* mArgArr;
	int mArgArrSize;
	int mHighestPrime;
	
	void mSieve();
	void mReduce();
	void mInitialize();

	//latency and timing
	double mDiff;
	double mLatency;
};

#endif

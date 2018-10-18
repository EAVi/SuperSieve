#include "sieve.h"

Sieve::Sieve(int n, int np, int rank)
	: mMaxPrime(n)
	, mProcessSize(np)
	, mProcessRank(rank)
{
	mList = NULL;
	mListSize = 0;
	mArgArrSize = 0;
	mLatency = 0.0;
	mDiff = 0.0;
	mArgArr = BinArray();
}

void Sieve::findPrimes()
{
	mInitialize();
	while (mArgArrSize < mMaxPrime)
	{
		mSieve();
	}
}

void Sieve::getList(int ** list, int * listsize)
{
	*list = mList;
	*listsize = mListSize;
}

void Sieve::clean()
{
	if(mList != NULL)
	{
		free(mList);
		mList = NULL;
	}
	mArgArr.clear();
}

double Sieve::getTiming()
{
	return mDiff;
}

double Sieve::getLatency()
{
	return mLatency;
}

void Sieve::mSieve()
{
	double begin = MPI_Wtime();
	double end;
	//find the job for the current processor
	int min = AssignMin(mListSize, mProcessSize, mProcessRank);
	int max = AssignMax(mListSize, mProcessSize, mProcessRank);
	
	//find size of mArgArr, then allocate size
	mArgArrSize = mHighestPrime * mHighestPrime + 2;//up to index 2^mH + 1
	if (mArgArrSize >= mMaxPrime)
		mArgArrSize = mMaxPrime + 1;
	
	mArgArr.allocate(mArgArrSize);

	//set mArgArr to all primes, except 0 and 1
	mArgArr.setConsonant(0);
	mArgArr.setConsonant(1);
	
	//the core of the sieve algorithm, go through the list and mark all consonants
	for (int i = mProcessRank; i < mListSize; i += mProcessSize)
	{
		int a = mList[i];
		if(a == 2) continue;
		for (int j = a*2; j < mArgArrSize; j += a)
		{
			mArgArr.setConsonant(j);
		}
	}

	end = MPI_Wtime();
	mDiff += end - begin;
	//Allreduce using MPI_BOR
	mReduce();
	begin = MPI_Wtime();
	mLatency += begin - end;
	
	//to save memory at the cost of more time,
	//count the number of primes before making a list
	mListSize = 1;
	for(int i = 3; i < mArgArrSize; i += 2)
	{
		if(mArgArr.get(i) == kPrime)
		{
			mListSize++;
		}
	}
	//create the next prime list
	free(mList);
	mList = (int*)malloc(sizeof(int) * mListSize);
	mList[0] = 2;
	for(int i = 3, j = 1; i < mArgArrSize && j < mListSize; i += 2)
	{
		if(mArgArr.get(i) == kPrime)
			mList[j++] = i;
	}
	
	mHighestPrime = mList[mListSize - 1];
	end = MPI_Wtime();
	mDiff += end - begin;
}

void Sieve::mReduce()
{
	mArgArr.reduce();
}

void Sieve::mInitialize()
{
	mListSize = 1;
	mList = (int*)malloc(sizeof(int) * mListSize);
	mList[0] = 2;
	mHighestPrime = 2;

}

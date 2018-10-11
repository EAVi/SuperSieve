#include "sieve.h"

Sieve::Sieve(int n, int np, int rank)
	: mMaxPrime(n)
	, mProcessSize(np)
	, mProcessRank(rank)
{
	mList = NULL;
	mListSize = 0;
	mArgArr = NULL;
	mArgArrSize = 0;
	mLatency = 0.0;
	mDiff = 0.0;
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
	if(mArgArr != NULL)
	{
		free(mArgArr);
		mArgArr = NULL;
	}
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
	
	//reallocate mArgArr
	mArgArrSize = mHighestPrime * mHighestPrime + 2;//up to index 2^mH + 1
	if (mArgArrSize >= mMaxPrime)
		mArgArrSize = mMaxPrime + 1;
	
	if(mArgArr != NULL)
		free(mArgArr);
	mArgArr = (char*)malloc(sizeof(char) * mArgArrSize);
	
	//set mArgArr to all primes, except 0 and 1
	mArgArr[0] = kConsonant;
	mArgArr[1] = kConsonant;
	for (int i = 2; i < mArgArrSize; ++i)
		mArgArr[i] = kPrime;
	
	//the core of the sieve algorithm, go through the list and mark all consonants
	for (int i = min; i < max; ++i)
	{
		int a = mList[i];
		//if(a == 2) break;
		for (int j = a*2; j < mArgArrSize; j += a)
		{
			mArgArr[j] = kConsonant;
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
		if(mArgArr[i] == kPrime)
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
		if(mArgArr[i] == kPrime)
			mList[j++] = i;
	}
	
	mHighestPrime = mList[mListSize - 1];
	end = MPI_Wtime();
	mDiff += end - begin;
}

void Sieve::mReduce()
{
	char* temparr = (char*)malloc(sizeof(char)* mArgArrSize);
	MPI_Allreduce(mArgArr, temparr, mArgArrSize, MPI_CHAR, MPI_BOR, MPI_COMM_WORLD);
	free(mArgArr);
	mArgArr = temparr;
}

void Sieve::mInitialize()
{
	mListSize = 1;
	mList = (int*)malloc(sizeof(int) * mListSize);
	mList[0] = 2;
	mHighestPrime = 2;

}

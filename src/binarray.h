#ifndef BINARRAY_H
#define BINARRAY_H

#include <cstdlib>
#include <cstdio>
#include <mpi.h>

const bool kPrime = false;
const bool kConsonant = true;
const bool kNonPrime = true;

class BinArray
{
public:
	BinArray();
	BinArray(int binsize);	
	
	void setConsonant(int i);	//set i to consonant
	bool get(int i);	//return whether address i is prime
	void reduce();	//use MPI_Allreduce
	void clear();	//deallocate array
	void tare();	//set all to 0
	void allocate(int binsize); //allocate array
	
	int getSize();//return mBitSize
	void printArray();//print it all
	
private:
	char* mData;
	int mBitSize;
	int mByteSize;
};

#endif

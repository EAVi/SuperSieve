#ifndef BINARRAY_H
#define BINARRAY_H

#include <cstdlib>
#include <cstdio>

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
	void deallocate();//deallocate array, without setting zero values
	void clear();	//deallocate array, setting zero values
	void tare();	//set all to 0
	void allocate(int binsize); //allocate array
	
	int getSize();//return mBitSize
	void printArray();//print it all
	char* getData();//return data array pointer
	void setCUDAData(char** c);
	
private:
	char* mData;
	char** mCUDA_Data;
	int mBitSize;
	int mByteSize;
};

#endif

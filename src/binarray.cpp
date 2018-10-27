#include <iostream>
#include "binarray.h"
#include "mpi.h"
#include "cuda_sieve.h"

BinArray::BinArray()
{
	mData = NULL;
	mCUDA_Data = NULL;
	mBitSize = 0;
	mByteSize = 0;
}

BinArray::BinArray(int binsize)
{
	mData = NULL;
	mCUDA_Data = NULL;
	mBitSize = 0;
	mByteSize = 0;
	allocate(binsize);
}

void BinArray::setConsonant(int i)
{
	if (i >= mBitSize) return;
	if (mData == NULL) return;
	/*
		since the only even prime is 2, the set ignores the lowest bit 
		of the address and only goes through odds 
		
		1 bit is thrown out
		3 bits are used for a shift
		the rest are used for the byte address
		
	*/
	if (!(i & 1))//if even
		return;
	
	const int shiftmask = 14; //0b1110
	int shift = (i & shiftmask) >> 1;
	char shiftedbit = (char)kConsonant << shift;
	
	int byteaddress = i >> 4;
	
	mData[byteaddress] |= shiftedbit;
}

bool BinArray::get(int i)
{
	if (mData == NULL) return kPrime;
	if (i >= mBitSize) 
	{
		printf("Error: index %i >= %i\n\n", i, mBitSize);
		return kConsonant;
	}
	/*
		since 2 is a prime, and the function ignores even numbers,
		2 needs to be hardcoded as prime
	*/
	if (i == 2)
		return kPrime;
	else if (!(i & 1))//if even
		return kConsonant;
	
	const int shiftmask = 14; //0b1110
	int shift = (i & shiftmask) >> 1;
	
	int byteaddress = i >> 4;
	
	if ((mData[byteaddress] >> shift) & kConsonant)
		return kConsonant;
	else return kPrime;
}

void BinArray::reduce()
{
	char* temparr = (char*)malloc(sizeof(char) * mByteSize);
	MPI_Allreduce(mData, temparr, mByteSize, MPI_CHAR, MPI_BOR, MPI_COMM_WORLD);
	deallocate();
	mData = temparr;
}

void BinArray::deallocate()
{
	if(mData == NULL)
		return;
//	else if (mCUDA_Data != NULL)
//	{
//		printf("%s\n", "calling cudafree");
//		launch_cudafree(mCUDA_Data);
//		fflush(stdin);
//		mData = NULL;
//		mCUDA_Data = NULL;
//	}
//	else
	{
		free(mData);
		mData = NULL;
	}
	if (mCUDA_Data != NULL)
	{
		*mCUDA_Data = NULL;
		mCUDA_Data = NULL;
	}
}

void BinArray::clear()
{
	deallocate();
	mBitSize = 0;
	mByteSize = 0;
}

void BinArray::tare()
{
	for(int i = 0; i < mByteSize; ++i)
		mData[i] = 0;
}

void BinArray::allocate(int binsize)
{
	if (binsize  == 0) return;//cannot allocate an empty array
	
	else if (mData != NULL) clear();//deallocate in order to reallocate
	
	mBitSize = binsize;
	mByteSize = 1 + (binsize >> 4);
	mData = (char*)malloc(sizeof(char) * mBitSize);
	
	if(mData == NULL)//an issue on malloc
	{
		mBitSize = 0;
		mByteSize = 0;
		return;
	}
	
	tare();
}

int BinArray::getSize()
{
	return mBitSize;
}

void BinArray::printArray()
{
	for(int i = 1, j = mBitSize; i < j; i += 2)
	{
		printf("%i, ", get(i));

	}
	printf("\n");
}

char* BinArray::getData()
{
	return mData;
}

void BinArray::setCUDAData(char** c)
{
	if (c == NULL) return;
	if (*c == NULL) return;
	if(mData != NULL)
	{
		free(mData);
		mData = *c;
		mCUDA_Data = c;
	}
}

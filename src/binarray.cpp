#include "binarray.h"

BinArray::BinArray()
{
	mData = NULL;
	mBitSize = 0;
	mByteSize = 0;
}

BinArray::BinArray(int binsize)
{
	mData = NULL;
	mBitSize = 0;
	mByteSize = 0;
	allocate(binsize);
}

void BinArray::setConsonant(int i)
{
	/*
		since the only even prime is 2, the set ignores the lowest bit 
		of the address and only goes through odds 
		
		1 bit is thrown out
		3 bits are used for a shift
		the rest are used for the byte address
		
	*/
	
	const int shiftmask = 14; //0b1110
	int shift = (i | shiftmask) >> 1;
	char shiftedbit = (char)kConsonant << shift;
	
	int byteaddress = i >> 4;
	
	mData[byteaddress] |= shiftedbit;
}

bool BinArray::get(int i)
{
	/*
		since 2 is a prime, and the array ignores even numbers,
		2 needs to be included
	*/
	if (i == 2)
		return kPrime;
	
	const int shiftmask = 14; //0b1110
	int shift = (i | shiftmask) >> 1;
	char shiftedbit = (char)kConsonant << shift;
	
	int byteaddress = i >> 4;
	
	return (shiftedbit & mData[byteaddress]);
}

void BinArray::reduce()
{
	//MPI_AllReduce in here or whatever
}

void BinArray::clear()
{
	if(mData == NULL)
		return;
	else 
	{
		free(mData);
		mData = NULL;
		mBitSize = 0;
		mByteSize = 0;
	}
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
	mBitSize = 1 + (binsize >> 4);
	mData = (char*)malloc(sizeof(char) * mBitSize);
	
	if(mData == NULL)//an issue on malloc
	{
		mBitSize = 0;
		mByteSize = 0;
		return;
	}
	
	tare();
}

int BinArray::getBitSize()
{
	return mBitSize;
}
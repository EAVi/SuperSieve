#ifndef BINARRAY_H
#define BINARRAY_H

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
	
private:
	char* mData;
	int mBitSize;
	int mByteSize;
};

#endif
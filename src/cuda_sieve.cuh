#ifndef CUDA_SIEVE_CUH
#define CUDA_SIEVE_CUH

#include "binarray.h"

//reduce from a character array of bytes utilizing only 1 bit
//to a character array using all 8 bits
__global__
void cuda_reduce(char* bigbinarray, char* smallbinarray, const int smallbinarray_size);

__global__
void cuda_sieve(char* binarray, const int  binarray_size, const int * primelist, const int primelist_size, const int  start, const int  stepsize);

__device__
void cuda_binarray_markconsonant(char* binarray, const int & binarray_size, int i);

#endif

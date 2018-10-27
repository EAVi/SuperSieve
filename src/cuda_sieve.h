#ifndef CUDA_SIEVE_H
#define CUDA_SIEVE_H

#include "binarray.h"

bool launch_cuda_sieve(char** binarray, const int & binarray_size, const int * primelist, const int & primelist_size ,const int & start, const int & stepsize);

#endif

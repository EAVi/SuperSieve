#include "cuda_sieve.h"
#include "cuda_sieve.cuh"

bool launch_cuda_sieve(char** binarray, const int & binarray_size, const int * primelist, const int & primelist_size, const int & start, const int & stepsize)
{
	cudaError_t err;//error checking variable

	if(primelist == NULL || *binarray == NULL)
	{
		printf("NULL pointer passed\n");
		return 0;
	}
	if(primelist_size <= 0)
	{
		printf("primelist size is non-positive");
		return 0;
	}

	//memory size variables
	int bytesize = 1 + (binarray_size >> 4);
	size_t device_freemem = 0;
	size_t device_totalmem = 0;
	size_t membin = (sizeof(char) * bytesize);
	size_t memlist = (sizeof(int) * primelist_size);
	size_t total_memneeded = membin + memlist;

	//the data to be allocated to device
	int * device_primelist = NULL;
	char * device_binarray = NULL;

	//check to make sure there's enough memory on the device
	cudaMemGetInfo(&device_freemem, &device_totalmem);
	if (total_memneeded > device_freemem)//not enough mem on device
	{
		printf("Not enough memory on CUDA device total: %i, free: %i, needed: %i\n",
			device_totalmem, device_freemem, total_memneeded);
		return false;
	}

	//allocate data onto to device
	err = cudaMalloc(&device_primelist, memlist);
	if(err != cudaSuccess)
	{
		printf("malloc failed on device primelist (%i)\n", err);
		return false;
	}
	err = cudaMalloc(&device_binarray, membin);
	if(err != cudaSuccess)
	{
		printf("malloc failed on device binarray (%i)\n", err);
		return false;
	}

	//copy primelist to device
	err = cudaMemcpy(device_primelist, primelist, memlist, cudaMemcpyHostToDevice);
	if(err != cudaSuccess)
	{
		printf("primelist memcpy failed(%i)\n", err);
		return false;
	}

	//call kernel
	cuda_sieve<<<1,1>>>(device_binarray, binarray_size, device_primelist, primelist_size, start, stepsize);
	cudaDeviceSynchronize();

	//copy binarray to cpu
	err = cudaMemcpy(*binarray, device_binarray, membin, cudaMemcpyDeviceToHost);
	if (err != cudaSuccess)
	{
		printf("binarray memcpy failed (%i)\n", err);
		return false;
	}

	//deallocate
	if (device_binarray != NULL)
	{
		err = cudaFree(device_binarray);
		if (err != cudaSuccess)
		{
			printf("device binarray free failed(%i)\n", err);
			return false;
		}
		device_binarray = NULL;
	}
	if (device_primelist != NULL)
	{
		err = cudaFree(device_primelist);
		if (err != cudaSuccess)
		{
			printf("device primelist free failed(%i)\n", err);
			return false;
		}
		device_primelist = NULL;
	}
	return true;
}

__global__
void cuda_sieve(char* binarray, const int  binarray_size, const int * primelist, const int primelist_size, const int  start, const int  stepsize)
{
	//first prime should always be 2, otherwise, something's wrong
	if (primelist[0] != 2) 
	{
		printf("primelist was not properly passed\n");
		return;
	}

	for(int i = start; i < primelist_size; i += stepsize)
	{
		int a = primelist[i];
		if (a == 2) continue; //skip evens
		for(int j = a*3; j < binarray_size; j += a*2)
		{
			cuda_binarray_markconsonant(binarray, binarray_size, j);
		}
	}
}

__device__
void cuda_binarray_markconsonant(char* binarray, const int & binarray_size, int i)
{
	//a cuda version of the markconsonant function in binarray.cpp
	if(i >= binarray_size)return;
	if(!(i & 1))return; //skip evens

	int shift = (i >> 1) & 7;
	binarray[i >> 4] |= (1 << shift);
}

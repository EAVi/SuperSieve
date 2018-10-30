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
	size_t membin = (sizeof(char) * bytesize);//output binarray size
	size_t memmeta = (sizeof(char) * (bytesize << 3));//matadata bigbinarr size
	size_t memlist = (sizeof(int) * primelist_size);//input primelist size
	size_t total_memneeded = membin + memlist;

	//the data to be allocated to device
	int * device_primelist = NULL;
	char * device_metaarray = NULL;
	char * device_binarray = NULL;

	//problem distribution across the device
	int blocksize = 512;//some arbitrary number that seems to be a sweetspot for the 1050Ti
	int numblocks = 1;//(primelist_size + blocksize - 1)/blocksize;//distribute across these many thread blocks

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
	err = cudaMalloc(&device_metaarray, memmeta);
	if(err != cudaSuccess)
	{
		printf("malloc failed on device metaarray (%i)\n", err);
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
	cuda_sieve<<<numblocks,blocksize>>>(device_metaarray, binarray_size, device_primelist, primelist_size, start, stepsize);

	cuda_reduce<<<numblocks,blocksize>>>(device_metaarray, device_binarray, binarray_size);

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
	if (device_metaarray != NULL)
	{
		err = cudaFree(device_metaarray);
		if (err != cudaSuccess)
		{
			printf("device primelist free failed(%i)\n", err);
			return false;
		}
		device_metaarray = NULL;
	}
	return true;
}

__global__
void cuda_reduce(char* bigbinarray, char* smallbinarray, const int smallbinarray_size)
{
	int size =  smallbinarray_size >> 1; //cut the size in half to represent the bigbinarray size
	int bytestart = (threadIdx.x + blockDim.x * blockIdx.x) << 3;
	int bytestepsize = (blockDim.x * gridDim.x) << 3;
	//each thread writes in multiples of 8 to prevent race condition
	for(int i = bytestart; i < size; i += bytestepsize)
	{
//		printf("REDUCE tid %i, bid %i, i %i\n", threadIdx.x, blockIdx.x, i);
		for(int j = i, k = i + 8; j < k; ++j)
		{
			
			if (bigbinarray[j] == 0)//0 is prime, 1 is consonant; skip on prime
				continue;
			int oddnumber = 1 + (j << 1);//the odd number represented by the halved number
//			printf("reducing number %i\n", oddnumber);
			cuda_binarray_markconsonant(smallbinarray, smallbinarray_size, oddnumber);
		}
	}
}

__global__
void cuda_sieve(char* binarray, const int  binarray_size, const int * primelist, const int primelist_size, const int  start, const int  stepsize)
{
//	int metasize = binarray_size >> 1;
	int devicestepsize = gridDim.x * blockDim.x * stepsize;
	int devicestart = (threadIdx.x + start*blockDim.x) + blockDim.x * blockIdx.x * stepsize;
//printf("threadid %i start %i, step %i\n", threadIdx.x, devicestart, devicestepsize);
	//first prime should always be 2, otherwise, something's wrong
	if (primelist[0] != 2) 
	{
		printf("primelist was not properly passed\n");
		return;
	}
	int iterationcount = 0;
	for(int i = devicestart; i < primelist_size; i += devicestepsize)
	{
		iterationcount++;
		int a = primelist[i];
		if (a == 2) continue; //skip multiples of 2
		for(int j = a*3; j < binarray_size; j += a*2)
		{
			int metaindex = j >> 1;
			binarray[metaindex] = 1;
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

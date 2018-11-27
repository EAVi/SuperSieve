# SuperSieve
##### Here's the Assignment:
>Write a program to find all prime numbers up to some value M, using the Sieve of Eratosthenes algorithm we discussed in class (look up details on the web)
>You will be parallelizing this algorithm using MPI, OpenMP and CUDA (details will be discussed, Week 2 we will discuss MPI, others lateri.
>You want to evaluate difficulty of getting it to work and how long calculating takes under different methods and on different compute platforms (single 8 core machine, a set of machines on the network, a single GPU)

##### Here's what I've got so far:

- [x] Sieve of Eratosthenes up to _M_
- [x] Using MPI
- [x] Using CUDA

##### Arguments:
1. integer _M_, find primes up to _M_, default 100
2. integer _S_, start using CUDA functions when problem size reaches _S_, 0 uses CUDA in all cases, -1 never uses CUDA (default).

##### Running:
Here's what I use if I plan on running CUDA on multiple nodes:
```
mpirun -np 10 -hostfile hostfile --map-by ppr:1:node ssieve 1000000000 0
```
And if I do not plan on using CUDA:
```
mpirun -hostfile hostfile ssieve 1000000000
```
# Performance:
To get any noticeable speedup, a large value M is needed. 

| M | 1 CPU | 1 GPU | 4 CPU cores | 40 CPU cores | 10 GPUs |
| --- | --- | --- | --- | --- | --- |
| 10<sup>8</sup> |  1.608491 s  | 1.386432 s | 1.105095 s | 1.374422 s | 1.503408 s |
| 10<sup>9</sup> |  22.097686 s  | 10.668882 s | 11.947728 s | 10.336707 s | 13.235919 s |
| 1.5\*10<sup>9</sup> |  33.700170 s  | 15.902881 s | 24.185493 s |  20.957107 s | 23.576214 s |

There were a lot of difficulties when trying to parallelize the algorithm: Workload, network latency, and GPU staging are the big 3. I was able to get a fairly good speedup with multiple CPUs, but distrbituing the problem on the school network introduced some slowdowns, even after implementing a structure that reduced the amount of data by <sup>1</sup>&frasl;<sub>16</sub>th.

I was able to get more remarkable results with my [SuperConway](https://github.com/EAVi/SuperConway) algorithm.

### Complexity:
This specific implementation of the algorithm is &in; __O__(&pi;(_M_)<sup>2</sup>), where &pi;(_M_) is the number of primes up to _M_

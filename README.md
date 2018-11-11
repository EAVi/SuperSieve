# SuperSieve
##### Here's the Assignment:
>Write a program to find all prime numbers up to some value M, using the Sieve of Eratosthenes algorithm we discussed in class (look up details on the web)
>You will be parallelizing this algorithm using MPI, OpenMP and CUDA (details will be discussed, Week 2 we will discuss MPI, others lateri.
>You want to evaluate difficulty of getting it to work and how long calculating takes under different methods and on different compute platforms (single 8 core machine, a set of machines on the network, a single GPU)

##### Here's what I've got so far:

- [x] Sieve of Eratosthenes up to M
- [x] Using MPI
- [x] Using CUDA


##### Running:
Here's what I use if I plan on running CUDA on multiple nodes:
```
mpirun -np 10 -hostfile hostfile --map-by ppr:1:node ssieve 1000000000 0
```
And if I do not plan on using CUDA:
```
mpirun -hostfile hostfile ssieve 1000000000
```

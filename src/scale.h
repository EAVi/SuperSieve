#ifndef SCALE_H
#define SCALE_H


//functions that depend on problem size
//allows for easy scalability

//assign the minimum for the current process
int AssignMin(const int& arraysize, const int& np, const int& id);

//assign the maximum for the current process
//note to use < to prevent overlap between problems
int AssignMax(const int& arraysize, const int& np, const int& id);

//max - min
//get the size of assignment 
int AssignSize(const int& arraysize, const int& np, const int& id);

//find the process which was assigned this index
int FindID(const int& arraysize, const int& np, const int& index);

#endif
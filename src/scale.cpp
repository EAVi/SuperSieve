#include "scale.h"

int AssignMin(const int& arraysize, const int& np, const int& id)
{
	return (arraysize * id / np);
}

int AssignMax(const int& arraysize, const int& np, const int& id)
{
	int nextid = id + 1;
	//return the next minimum
	return AssignMin(arraysize, np, nextid);
}

int AssignSize(const int& arraysize, const int& np, const int& id)
{
	//max - min
	return (AssignMax(arraysize, np, id) 
			-AssignMin(arraysize, np, id));
}

int FindID(const int& arraysize, const int& np, const int& index)
{
	return ((index + 1) * np - 1) / arraysize;
}
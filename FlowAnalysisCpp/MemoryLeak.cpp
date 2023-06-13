#include <stdio.h>

class IOException {};

int* readIntegerArray(FILE* file, int* pSize)
{
	fscanf(file, "%d", pSize);
	int* data = new int[*pSize];
	for (int i = 0; i < *pSize; i++) {
		if (fscanf(file, "%d", &data[i]) == EOF) {
			throw IOException();
		}
	}
	return data;
}

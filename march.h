#ifndef MARCH_H
#define MARCH_H

void marchingCubes(int dims[3], float *potential, int shift[3], int indexOffset, float *positions, unsigned int *faces, unsigned int &positionIndex, unsigned int &faceIndex);
bool collideBoxEther(int dims[3], float *potential, float *boxSpec);

#endif

#include "tensor.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Calculate squared vector
 * @param  m   Vector
 * @param  len Vector's length
 * @return     Squared vector
 */
double* vecSqrt(double* m, int len)
{
	int n = len*2+1;
	double* s = (double *) malloc(n * sizeof(double));
	int j = 0;
	for (int i = 0; i < n; i++) {
		if ((i+1)%2 == 1) {
			s[i] = m[j] * m[j];
		} else {
			s[i] = 2 * m[j] * m[j+1];
			j++;
		}
	}
	return s;
}

/**
 * Scalar to vector multiplication
 * @param  s Scalar
 * @param  a Vector
 * @return   Scaled vector
 */
double* scalarMul(double s, double* a, int len)
{
	double* b = (double *) malloc(len * sizeof(double));
	for (int i = 0; i < len; ++i)
		b[i] = s*a[i];
	return b;
}
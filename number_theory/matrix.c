#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Calculate double data type squared matrix
 * @param  m   Matrix
 * @param  len Matrix's length
 * @return     Squared matrix
 */
double* squaredDouble(double* m, int len)
{
	int n = len + len - 1;
	double* s = (double *) malloc(n * sizeof(double));
	for (int i = 0; i < len; i++) {
		s[--n] = m[i] * m[i];
	}
	for (; n >= 0; n--) {
		s[n] = 0;
	}
	return s;
}

/**
 * Scalar to matrix multiplication
 * @param  s Scalar
 * @param  a Matrix
 * @return   Scaled matrix
 */
double* scalarMul(double s, double* a, int len)
{
	double* b = (double *) malloc(len * sizeof(double));
	for (int i = 0; i < len; ++i)
		b[i] = s*a[i];
	return b;
}
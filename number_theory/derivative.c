#include "derivative.h"
#include "tensor.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Calculate derivative of vector a
 * @param  a Vector
 * @return   Derivative of a
 */
double* derivative(double* a, int len)
{
	double *d = (double *) malloc(len * sizeof(double));
	d[0] = 0;
	int j = len;
	for (int i = 1; i < len; i++){
		d[i] = a[i-1] * --j;
	}

	// //test
	// for (int i = 0; i < len; i++){
	// 	printf("%lf ", a[i]);
	// }
	// printf("==> ");
	// for (int i = 0; i < len; i++){
	// 	printf("%lf ", d[i]);
	// }
	// printf("\n");

	return d;
}

/**
 * Product derivative
 * @param  a 	Vector a
 * @param  b 	Vector b
 * @param  lena Length of vector a
 * @param  lenb Length of vector b
 * @return   	Product vector
 */
double* product(double* a, double* b, int lena, int lenb)
{
	double *f = a, *g = b;
	int lenf = lena, leng = lenb;

	if (lenf > leng) {
		f = b, g = a;
		lenf = lenf+leng;
		leng = lenf-leng;
		lenf -= leng;
	}

	double *df = derivative(f, lenf), *dg = derivative(g, leng);
	int lenh = lenf+leng-2;
	double *h = (double *) malloc(lenh * sizeof(double));

	for (int i = 0; i < lenh; i++) {
		h[i] = 0;
		int j = i;
		for (int k = 1; k < lenf; k++) {
			if (j >= leng) {
				--j; continue;
			}
			// printf("%d: %lf*%lf\n", i, df[k], g[j]);
			h[i] += df[k] * g[j];
			if (--j < 0) break;
		}
		j = i+1;
		for (int k = 0; k < lenf; k++) {
			if (j >= leng) {
				--j; continue;
			}
			// printf("%d: %lf*%lf\n", i, f[k], dg[j]);
			h[i] += f[k] * dg[j];
			if (--j == 0) break;
		}
	}

	// // test
	// for (int i = 0; i < lenh; ++i)
	// {
	// 	printf("%lf ", h[i]);
	// }
	// printf("\n");

	return h;
}

/**
 * Calculate quotient of vector a and b
 * @param  a 	Vector a
 * @param  b 	Vector b
 * @param  lena Length of vector a
 * @param  lenb Length of vector b
 * @return   	Quotient vector
 */
double** quotient(double *a, double *b, int lena, int lenb)
{
	double *f = a, *g = b;
	int lenf = lena, leng = lenb;

	if (lenf > leng) {
		f = b, g = a;
		lenf = lenf+leng;
		leng = lenf-leng;
		lenf -= leng;
	}

	double *df = derivative(f, lenf), *dg = derivative(g, leng);
	int lenh = lenf+leng-2;
	double *h = (double *) malloc(lenh * sizeof(double));

	for (int i = 0; i < lenh; i++) {
		h[i] = 0;
		int j = i;
		for (int k = 1; k < lenf; k++) {
			if (j >= leng) {
				--j; continue;
			}
			h[i] += df[k] * g[j];
			if (--j < 0) break;
		}
		j = i+1;
		for (int k = 0; k < lenf; k++) {
			if (j >= leng) {
				--j; continue;
			}
			h[i] -= f[k] * dg[j];
			if (--j == 0) break;
		}
	}

	double **q = (double **) malloc(2 * sizeof(double));
	q[0] = h;
	q[1] = vecSqrt(b, lenb);

	return q;
}
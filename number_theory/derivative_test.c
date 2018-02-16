#include "derivative.h"
#include <stdio.h>
#include <stdlib.h>
#define True '1'
#define False '0'

void printX(double *mtr, int len, char asInt)
{
	int x = len-1;
	for (int i = 0; i < len; i++) {
		if (asInt == True)
			printf("%d*x^%d", (int) mtr[i], x--);
		else
			printf("%lf*x^%d", mtr[i], x--);
		if (i < len-1) printf(" + ");
	}
	printf("\n");
}


int main(int argc, char const *argv[])
{
	// double a[3] = {-6, -1, -7};
	// double b[4] = {2, 3, -2, -5};
	// double *c = product(a, b, 3, 4);
	// printX(a, 3, True);
	// printX(b, 4, True);
	// printX(c, 5, True);

	// double a[2] = {-1, -1};
	// double b[4] = {7, 2, -1, -6};
	// double *c = product(a, b, 2, 4);
	// printX(a, 2, True);
	// printX(b, 4, True);
	// printX(c, 4, True);

	// double a[3] = {7, -1, 3};
	// double b[3] = {6, 3, 6};
	// double *c = product(a, b, 3, 3);
	// printX(a, 3, True);
	// printX(b, 3, True);
	// printX(c, 4, True);

	// double a[2] = {2, 2};
	// double b[4] = {2, -3, 3, 3};
	// double **c = quotient(a, b, 2, 4);
	// printX(a, 2, True);
	// printX(b, 4, True);
	// printX(c[0], 4, True);
	// printX(c[1], 7, True);

	// double a[2] = {5, 3};
	// double b[2] = {4, -7};
	// double **c = quotient(a, b, 2, 2);
	// printX(a, 2, True);
	// printX(b, 2, True);
	// printX(c[0], 2, True);
	// printX(c[1], 3, True);

	double a[2] = {-5, 1};
	double b[3] = {1, -4, -5};
	double **c = quotient(a, b, 2, 3);
	printX(a, 2, True);
	printX(b, 3, True);
	printX(c[0], 3, True);
	printX(c[1], 5, True);

	return 0;
}
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
	double a[2] = {-4, -1};
	double b[3] = {3, 5, 0};
	double *c = product(a, b, 2, 3);
	printf("Input\n");
	printX(a, 2, True);
	printX(b, 3, True);
	printf("Output\n");
	printX(c, 3, True);
	
	// double a[2] = {6, 7};
	// double b[3] = {-7, 1, 5};
	// double **c = quotient(a, b, 2, 3);
	// printf("Input\n");
	// printf("Numerator    : ");printX(a, 2, True);
	// printf("Denominator  : ");printX(b, 3, True);
	// printf("Output\n");
	// printf("Numerator    : ");printX(c[0], 3, True);
	// printf("Denominator  : ");printX(c[1], 5, True);

	return 0;
}
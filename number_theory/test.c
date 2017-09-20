#include <stdlib.h>
#include <stdio.h>
#include "polynomial.h"

int main(int argc, char const *argv[])
{
    /* code */
    // Evaluate x^3+12x^2+4x+36, where x = 3
    unsigned int n = 3;
    double a[4] = {36, 4, 12, 1};
    double x = 3;
    double result = horner(n, a, x);
    printf("(3)^3+12(3)^2+4(3)+36 = %f\n", result);
    return 0;
}
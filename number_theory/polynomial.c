#include <stdio.h>

/**
 * Evaluate polynomial using horner rule
 * Running Time: %THETA(n)
 * @param  n Number of nominal or term
 * @param  a Array of constants
 * @param  x Value of x
 * @return   Evaluated result of polynomial
 */
double horner(unsigned int n, double *a, double x)
{
    double p = a[n];
    for (int i = n - 1; i >= 0; --i)
    {
        p = a[i] + (x * p);
    }
    return p;
}
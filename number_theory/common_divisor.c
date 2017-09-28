#include <math.h>
#include <stdio.h>

/**
 * Beauty but useless: Find the greatest common divisor of two or more number.
 * References:
 * 	Introduction to Algorithm 3rd Edition
 * 	Introduction to Design and Analysis Algorithm
 */

/* prototype declarations */
unsigned int euclidGCD(unsigned int a, unsigned int b);

/**
 * Main program
 */
int main(int argc, char const *argv[])
{
    printf("%d\n", euclidGCD(30, 21));
    return 0;
}

/**
 * Calculate greatest common divisor using Euclid algorithm
 * Found by Euclid (circa 300 B.C).
 *
 * Theorem
 * We could find common divisor (d) of x and y
 * by iterating each division remainder.
 *
 *     gcd(a, b) = gcd(b, a mod b)
 *
 *  where,
 *         b + (a mod b) = b + (a - b [a/b])
 *
 * Then,
 *     gcd(b, a mod b) = gcd(a, b)
 *
 * @param  a First number
 * @param  b Number smaller than a
 * @return   Common divisor
 */
unsigned int euclidGCD(unsigned int a, unsigned int b)
{
    if (b == 0)
        return a;
    else
        return euclidGCD(b, a % b);
}
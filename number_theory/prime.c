#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/**
 * Beauty but useless: Find prime numbers in range x to y.
 * References:
 * 	Introduction to Algorithm 3rd Edition
 * 	Introduction to Design and Analysis Algorithm
 */

/* Prototype declarations */
void sieve(unsigned int x, unsigned int y);

/**
 * Main program
 */
int main(int argc, char const *argv[])
{
	unsigned int x = 0, y = 0, n, i;
	char *c;

	if (argc >= 2) {
		c = argv[1]; n = strlen(argv[1]) - 1; i = 0;
		while(c[i] != '\0') {
			x += (c[i] - 48) * pow(10, n);
			i++; n--;
		}
		c = argv[2]; n = strlen(argv[2]) - 1; i = 0;
		while(c[i] != '\0') {
			y += (c[i] - 48) * pow(10, n);
			i++; n--;
		}

		sieve(x, y);
	}
	return 0;
}

/**
 * Find prime number in range x to y using Sieve Eratosthenes method.
 * Remove any number which are factor of i to n, where n <= y.
 *
 * @param x Start number in sequence
 * @param y last number in sequence
 */
void sieve(unsigned int x, unsigned int y)
{
	unsigned int *tmp = (unsigned int *) malloc((y - x + 1) * sizeof(unsigned int));
	if (x >= 2 && y > x)
	{
		for (int i = 0; i < (y - x + 1); i++)
		{
			tmp[i] = x + i;
		}

		unsigned int j;
		for (int i = 0; i < (sqrt(y) - x + 1); i++)
		{
			if (tmp[i] != 0)
			{
				j = tmp[i] * tmp[i];
				while (j <=  y)
				{
					tmp[j - x] = 0;
					j = j + tmp[i];
				}
			}
		}

		for (int i = 0; i < (y - x + 1); i++)
		{
			if (tmp[i] != 0) {
				printf("%d\n", tmp[i]);
			}
		}
	}
}
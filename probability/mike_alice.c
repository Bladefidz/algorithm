/**
 * Mike wants to go fishing this weekend to nearby lake.
 * His neighbor Alice (is the one Mike was hoping to ask out since long time)
 * is also planing to go to the same spot for fishing this weekend. The
 * probability that it will rain this weekend is p1. There are two possible
 * ways to reach the fishing spot (bus or train). The probability that Mike
 * will take the bus is pmb and that Alice will take the bus is pab.
 * Travel plans of both are independent of each other and rain.
 *
 * What is the probability prs
 * that Mike and Alice meet each other only (should not meet in bus or train)
 * in a romantic setup (on a lake in rain)?
 *
 * Input constraints
 * 	(0≤p1≤1), (0≤pab≤1), (0≤pmb≤1)
 *
 * Input format
 * 	First line: pmb
 * 	Second line: pab
 * 	Third line: p1
 *
 * Output format
 * 	prs, rounded up to six decimal places.
 */

#include <stdio.h>
#include <stdlib.h>
#define INT_BIT_LEN  10  // Max length of unsigned integer is 4,294,967,295

int charToInt(char c)
{
    int yes = -1;
    char numChar[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    int numInt[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    for (int i = 0; i <= 10; ++i)
    {
        if (c == numChar[i]) {
            yes = numInt[i];
            break;
        }
    }

    return yes;
}

int fastPower10(int power)
{
    // Max length of int is 2,147,483,648
    static int powers[10] = {1, 10, 100, 1000, 10000, 100000, 1000000,
        10000000, 100000000, 1000000000};

    return powers[power];
}

double bufferLineToDouble(const char *buffer)
{
	if (buffer[0] == '1')
		return 1.0;

	int len, p, value;
	len = p = value = 0;

	while (buffer[len] != '\n' && buffer[len] != '\0') {
		// printf("%c", buffer[len]);
		len++;
	}
	// printf(" ");

	if (len == 1 && buffer[0] == '0')
		return 0.0;

	// A buffer from line of file usually has carriage return (\r),
	// so its length always overhead by one.
	for (int i = len - 1; i >= 2; i--) {
		value += charToInt(buffer[i]) * fastPower10(p);
		p++;
	}

	double valueDouble = (double) value/fastPower10(len-2);
	// printf("%lf\n", valueDouble);
	return valueDouble;
}

int main(int argc, char const *argv[])
{
	double *p =(double *) malloc(3 * sizeof(double));
	int i = 0;
	char *buffer = (char *) malloc(INT_BIT_LEN * sizeof(char));

	// Get pmb, pab, and p1 from stream
	while(i < 3 && fgets(buffer, 12, stdin)) {
		double prob = bufferLineToDouble(buffer);
		if (prob < 0 || prob > 1.0) {
			printf("Given probability invalid\n");
			break;
		}
		p[i++] = prob;
	}

	// // Debug array of p
	// for (int i = 0; i < 3; i++) {
	// 	printf("%lf\n", p[i]);
	// }

	double p0not1 = p[0] * (1-p[1]);
	double p1not0 = p[1] * (1-p[0]);
	double marginalP10 = p0not1 + p1not0;
	double prs = marginalP10 * p[2];
	printf("%lf\n", prs);

	return 0;
}
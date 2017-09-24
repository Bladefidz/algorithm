#include <stdio.h>
#include <string.h>
#include "converter.h"

/**
 * Convert character into integer
 * @param  c Character
 * @return   Integer
 */
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

/**
 * Convert string to integer
 *
 * @param  str String
 * @return     Integer
 */
int stringToInteger(const char *str)
{
    int len, value, p;

    len = p = value = 0;

    while (str[len] != '\0') {
        len++;
    }

    for (int i = len - 1; i >= 0; i--) {
        value += charToInt(str[i]) * fastPower10(p);
        p++;
    }

    return value;
}

/**
 * Convert line buffer into integer
 *
 * @param  str Buffer line
 * @return     Integer
 */
int bufferLineToInteger(const char *buffer)
{
    int len, value, p;

    len = p = value = 0;

    while (buffer[len] != '\0') {
        len++;
    }

    // A buffer from line of file usually has carriage return (\r),
    // so its length always overhead by one.
    for (int i = len - 2; i >= 0; i--) {
        value += charToInt(buffer[i]) * fastPower10(p);
        p++;
    }

    return value;
}

/**
 * Fast power 10 for integer
 *
 * @param  base  Base number
 * @param  power Power number
 * @return       integer
 */
int fastPower10(int power)
{
    // Max length of int is 2,147,483,648
    static int powers[10] = {1, 10, 100, 1000, 10000, 100000, 1000000,
        10000000, 100000000, 1000000000};

    return powers[power];
}

/**
 * Convert string to unsigned integer
 *
 * @param  str String
 * @return     Integer
 */
unsigned int stringToUnsignedInteger(char *str)
{
    unsigned int len, c, i, j;

    len = c = 0;

    while (str[c] != '\0') {
        c++;
    }

    j = c;
    c = c - 1;

    for (i = 0; i < j; ++i) {
        len += ((unsigned int) charToInt(str[c])) * fastUnsignedPower10(i);
        c--;
    }

    return len;
}

/**
 * Fast unsigned power 10 for unsigned integer
 *
 * @param  base  Base number
 * @param  power Power number
 * @return       unsigned integer
 */
unsigned int fastUnsignedPower10(int power)
{
    // Max length of unsigned int is 4,294,967,295
    static unsigned int powers[10] = {1, 10, 100, 1000, 10000, 100000, 1000000,
        10000000, 100000000, 1000000000};

    return powers[power];
}
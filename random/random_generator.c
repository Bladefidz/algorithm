#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

/* Prototype declarations */
unsigned int *generateRandomIntArray(unsigned int length);
char *generateRandomCharArray(unsigned int length);
unsigned int fastUnsignedPower10(int power);
int toInt(char c);
void printIntArray(unsigned int *integer, unsigned int len);
void printIntFile(unsigned int *integer, unsigned int len);
void printChar(char *character, unsigned int len);
void putIntIntoFile(unsigned int *integer, unsigned int len);
void putCharIntoFile(char *character);

/**
 * Main program
 *
 * @param  argc Argument count
 * @param  argv Argument variable
 * @return      Integer
 */
int main(int argc, char const *argv[])
{
    unsigned int len, c, i, j;

    c = 0;
    len = 0;

    if (argc > 2) {
        while (argv[2][c] != '\0') {
            c++;
        }

        j = c;
        c = c - 1;

        for (i = 0; i < j; ++i) {
            len += ((unsigned int) toInt(argv[2][c])) * fastUnsignedPower10(i);
            c--;
        }

        if (len > 0) {
            if (strcmp(argv[1], "int") == 0) {
                if (argc >= 4) {
                    if (strcmp(argv[3], "new_line") == 0) {
                        printIntFile(generateRandomIntArray(len), len);
                    } else {
                        printIntArray(generateRandomIntArray(len), len);
                    }
                } else {
                    printIntArray(generateRandomIntArray(len), len);
                }
            } else if (strcmp(argv[1], "char") == 0) {
                generateRandomCharArray(len);
            }
        } else {
            printf("Data length should be more than 0\n");
        }
    } else if (argc == 2) {
        if (strcmp(argv[1], "-h") || strcmp(argv[1], "--help")) {
            printf("Generate random integer, characters or string\n\n"
                "Usage: ./random_generator <data type> <data length> "
                "<output>\n"
                "\nData type (required):\n"
                "   int: Integer.\n"
                "   char: Character.\n"
                "   str: String.\n"
                "\n Data length (required)\n:"
                "   unsigned integer: 1 - 4,294,967,295."
                "\n output (optional):\n"
                "   Array (default): Print as array.\n"
                "   new_line       : Print with 'new line' separator.\n"
                "   Path/to/file   : Save to file.\n"
                "\nExample:\n"
                "   ./random_generator int 10000\n"
                "   ./random_generator int 10000 output.txt\n");
        } else {
            printf("Error: Invalid argument!\n");
        }
    } else {
        printf("Type -h for help!\n");
    }

    return 0;
}

/**
 * Generate random array of integer
 *
 * @param  length Array length
 * @return        Array of integer
 */
unsigned int *generateRandomIntArray(unsigned int length)
{
    unsigned int *array;

    array = (unsigned int *) malloc(length * sizeof(unsigned int));

    for (unsigned int i = 0; i < length; ++i) {
        srand(time(NULL) * (i+1));
        array[i] = rand() % 100 + 1;
    }

    return array;
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

/**
 * Convert character into integer
 * @param  c Character
 * @return   Integer
 */
int toInt(char c)
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
 * Generate random character array
 *
 * @param  length Array length
 * @return        Array of character
 */
char *generateRandomCharArray(unsigned int length)
{
    char *c;

    return c;
}

/**
 * Print integer as array
 *
 * @param integer Integer value
 * @param len     Integer length
 */
void printIntArray(unsigned int *integer, unsigned int len)
{
    unsigned int i;

    printf("[");
    for (i = 0; i < len-1; ++i)
    {
        printf("%d ,", integer[i]);
    }
    printf("%d]\n", integer[i]);
}

/**
 * Print integer as file with new line separator
 *
 * @param integer Integer value
 * @param len     Integer length
 */
void printIntFile(unsigned int *integer, unsigned int len)
{
    for (int i = 0; i < len; ++i)
    {
        printf("%d\n", integer[i]);
    }
}

/**
 * Print char
 *
 * @param character Character
 */
void printChar(char *character, unsigned int len)
{
    unsigned int i = 0;

    printf("[");
    for (i = 0; i < len-1; ++i) {
        printf("%c", character[i]);
    }
    printf("%d]\n", character[i]);
}

/**
 * Put integer into file
 *
 * @param integer Integer value
 */
void putIntIntoFile(unsigned int *integer, unsigned int len)
{
    for (unsigned int i = 0; i < len; ++i)
    {
        printf("%d\n", integer[i]);
    }
}

/**
 * Put character into file
 *
 * @param character Character value
 */
void putCharIntoFile(char *character)
{
    unsigned int i = 0;

    while (character[i] != EOF) {
        printf("%c\n", character[i]);
        i++;
    }
}
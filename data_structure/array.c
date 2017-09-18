#include <stdlib.h>
#include <ctype.c>
#include <time.h>
#include <math.h>
#include "array.h"

/**
 * Generate random array of integer
 * @param  length Array length
 * @return        Array of integer
 */
int *generateRandomIntArray(int length)
{
    int *array;

    array = (int *) malloc(length * sizeof(int));

    for (int i = 0; i < length; ++i)
    {
        srand(time(NULL) * (i+1));
        array[i] = rand() % 100 + 1;
    }

    return array;
}

/**
 * Convert string to integer
 * @param  charArray string
 * @return           integer
 */
int stringToInt(char *charArray)
{
    int i, j, integer, len;

    i = 0;
    integer = 0;

    // Count string length
    while (charArray[i] != '\0') {
        i++;
    }

    j = 0;

    // Convert to integer
    while (i >= 0) {
        integer += toInt(charArray[i]) * (int) pow(10, j);
        j ++;
    }

    return integer;
}

/**
 * Transform string into array of integer
 * @param  charArray Array of character
 * @return           Array of integer
 */
int *stringToIntArray(char *charArray)
{
    /* Integer type */
    int *intArray, next, segmentLength, totalLength, i, j;
    char *string;

    /* List of character of given character array */
    struct CharList
    {
        char value;
        struct CharValue *next;
    };

    /* List of string length for each character segment separated by comma */
    struct CharSizeList
    {
        char value;
        struct CharSizeList *next;
    };

    i = 1;
    length = 0;
    CharList *CList = NULL;
    CharList *CurCList = NULL;
    CharSizeList *CSList = NULL;
    CharSizeList *CurCSList = NULL;

    /* Extract each character in character array */
    if (charArray[0] != '[') {
        CList = (struct CharList *) malloc(sizeof(struct CharList));
        next = 1;
        segmentLength = 0;
        totalLength = 0;

        // Get first numeric character
        while (next) {
            if (isNumericChar(charArray[i]) != -1) {
                next = 0;
                (*CList).value = charArray[i];
                (*CList).next = NULL;
                segmentLength = segmentLength + 1;
            }

            i++;
        }

        CurCList = (*CList).next;

        // Count allocation length
        while(charArray[i] != ']' || charArray[i] != EOF) {
            if (charArray[i] != ',') {

                // Get next numeric character
                if (isNumericChar(charArray[i]) != -1) {
                    (*CurCList).value = charArray[i];
                    (*CurCList).next = NULL;
                    CurCList = (*CurCList).next;
                    segmentLength = segmentLength + 1;
                }
            } else {
                if (CSList == NULL) {
                    // Save first string length into first node of CharSizeList
                    CSList = (struct CharSizeList *) malloc(
                        sizeof(struct CharSizeList));
                    (*CSList).value = segmentLength;
                    (*CSList).next = NULL;
                    CurCSList = (*CSList);
                } else {
                    // Save next string length into next node of CharSizeList
                    (*CurCSList).value = segmentLength;
                    (*CurCSList).next = NULL;
                    CurCSList = (*CurCSList).next;
                }

                // Reset segment length
                segmentLength = 0;
                // Increase total length
                totalLength = totalLength + 1;
            }

            i++;
        }

        // Allocate array
        intArray = (int *) malloc(totalLength * sizeof(int));

        // Covert every character into new array
        for (i = 0; i < totalLength; ++i)
        {
            CurCList = CList;
            CurCSList = CSList;
            segmentLength = (*CurCSList).value;
            intArray[i] = 0;

            // Sum each integer segment
            for (j = 0; j < (*CurCSList).value; ++j)
            {
                intArray[i] += toInt((*CurCList).value) * segmentLength * 10;
                CurCList = (*CurCList).next;
                segmentLength -= 1;
            }

            CurCSList = (*CurCSList).next;
        }

        return intArray;
    } else {
        return 0;
    }
}

/**
 * Check is character is numeric
 * @return Integer (0 or 1)
 */
int isNumericChar(char c)
{
    int yes = -1;
    char *numChar = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    for (int i = 0; i <= 10; ++i)
    {
        if (c == numChar[i]) {
            yes = 1;
            break;
        }
    }

    return yes;
}

/**
 * Convert character into integer
 * @param  c Character
 * @return   Integer
 */
int toInt(char c)
{
    int yes = -1;
    char *numChar = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    int *numInt = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}

    for (int i = 0; i <= 10; ++i)
    {
        if (c == numChar[i]) {
            yes = numInt[i];
            break;
        }
    }

    return yes;
}
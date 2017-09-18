#ifndef ARRAY_H_
#define ARRAY_H_

/**
 * Generate random array of integer
 * @param  length Array length
 * @return        Array of integer
 */
void *generateRandomIntArray(int length);

/**
 * Convert string to integer
 * @param  charArray string
 * @return           integer
 */
int stringToInt(char *charArray);

/**
 * Transform string into array of integer
 * @param  charArray Array of character
 * @return           Array of integer
 */
int *stringToIntArray(char *charArray);

/**
 * Check is character is numeric
 * @return Integer (0 or 1)
 */
int isNumericChar(char c);

/**
 * Convert character into integer
 * @param  c Character
 * @return   Integer
 */
int toInt(char c);

#endif
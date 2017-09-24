#ifndef CONVERTER_H_
#define CONVERTER_H_

/**
 * Prototype declarations
 */
int charToInt(char c);
int stringToInteger(const char *str);
int bufferLineToInteger(const char *str);
unsigned int stringToUnsignedInteger(char *str);
int fastPower10(int power);
unsigned int fastUnsignedPower10(int power);

#endif
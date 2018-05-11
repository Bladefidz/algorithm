#ifndef IO_FILE_H_
#define IO_FILE_H_
#include <stdio.h>

/* Global declarations */
typedef struct LineCount {
    unsigned int lines;
    unsigned int maxStrLen;
} LineCount;

/* Prototypes declarations */
unsigned int countLine(FILE *f);
LineCount *lineCounter(FILE *f);

#endif
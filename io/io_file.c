#include <stdlib.h>
#include <stdio.h>
#include "io_file.h"

/**
 * Count all lines in a file
 * @param  f Instance of FILE
 * @return   Count of lines
 */
unsigned int countLine(FILE *f)
{
    int c;
    unsigned int l = 1;

    while ((c = fgetc(f)) != EOF)
    {
        if (c == '\n')
        {
            l++;
        }
    }

    return l;
}

/**
 * Count line length and max string length
 * @param  f Instance of file
 * @return   Instance of LineCounter
 */
LineCount *lineCounter(FILE *f)
{
    int c;
    unsigned int l = 0;

    LineCount *LC = malloc(sizeof(LineCount));
    (*LC).lines = 0;
    (*LC).maxStrLen = 0;

    while ((c = fgetc(f)) != EOF)
    {
        l++;
        if (c == '\n')
        {
            (*LC).lines += 1;
            if ((*LC).maxStrLen == 0 || l > (*LC).maxStrLen)
            {
                (*LC).maxStrLen = l;
            }
            l = 0;
        }
    }

    (*LC).lines += 1;

    return LC;
}
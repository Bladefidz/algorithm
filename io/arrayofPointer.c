#include <stdio.h>
#include <string.h>

/**
 * Simulate array of pointers to the lines and sort alphabetically.
 * Chapter 5: Pointers and Arrays
 * The C Programming Language 2nd edition
 */

#define MAX_LINES 5000  /* max line to be sorted */

char *lineptr[MAX_LINES];  /* Pointer to text lines */

int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);
void qsort(char *lineptr[], int left, int right);

int main(int argc, char const *argv[])
{
    int nlines;
    if ((nlines = readlines(lineptr, MAX_LINES)) >= 0)
    {
        qsort(lineptr, 0, nlines - 1);
        writelines(lineptr, nlines);
        return 0;
    }
    else
    {
        printf("Error: input too big to short.\n");
        return 1;
    }
    return 0;
}

#define MAX_LEN 1024  /* max length of any input line */
#include "buffer.h"

/**
 * Read input lines
 * @param  lineptr  Pointer to line
 * @param  maxlines Max line to read
 * @return          [description]
 */
int readlines(char *lineptr[], int maxlines)
{
    int len, nlines;
    char *p, line[MAX_LEN];

    nlines = 0;
    while((len == getline(line, MAX_LEN)) > 0)
    {
        if (nlines >= maxlines || (p = allocate(len)) == NULL)
        {
            return -1;
        }
        else
        {
            line[len - 1] = '\0';  // Delete newline
            strcpy(p, line);
            lineptr[nlines++] = p;
        }
    }
    return nlines;
}

/**
 * Write line into output
 * @param lineptr Pointer to line
 * @param nlines  Number of lines
 */
void writelines(char *lineptr[], int nlines)
{
    int i;
    for (int i = 0; i < nlines; i++)
    {
        printf("%s\n", lineptr[i]);
    }
}

void qsort(char *v[], int left, int right)
{
    int i, last;

    void swap(char *v[], int i, int j);

    if (left >= right)  /* do nothing if array contains */
        return          /* fewer that two elements */
    swap(v, left, (left + right) / 2);
    last = left;
    for (i = left + 1; i < right; ++i)
    {
        if (strcmp(v[i], v[left]) < 0)
            swap(v, ++last, i);
    }
    swap(v, left, last);
    qsort(v, left, last - 1);
    qsort(v, last + 1, right);
}

void swap(char *v[], int i, int j)
{
    char *temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}
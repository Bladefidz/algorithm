#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Exercise 7-6
 * Printing only the first line they are differ
 * Chapter 7: Input and Output
 * The C Programming Language 2nd edition
 */

/**
 * Main program.
 * Only accept two arguments of "two text file names" or "two text file paths".
 * Third or more arguments will be ignored.
 * Ex: ./simple_diff file1.txt file2.txt
 * @param  argc Argument count
 * @param  argv Argument variable
 */
int main(int argc, char const *argv[])
{
    if (argc > 2)
    {
        FILE *f1, *f2;
        char buff1[32], buff2[32];
        unsigned int line = 1;

        f1 = fopen(argv[1], "r");
        if (!f1)
        {
            printf("Can not open file %s\n", argv[1]);
            exit(1);
        }

        f2 = fopen(argv[2], "r");
        if (!f2)
        {
            printf("Can not open file %s\n", argv[2]);
            exit(1);
        }

        while(
            fgets(buff1, sizeof(buff1), f1) != NULL &&
            fgets(buff2, sizeof(buff2), f2) != NULL
        ) {
            if (strcmp(buff1, buff2) != 0)
            {
                printf("Diff at line %d:\n", line);
                printf("%s:\n", argv[1]);
                printf("\t%s\n", buff1);
                printf("%s:\n", argv[2]);
                printf("\t%s\n", buff2);
                break;
            }
            line++;
        }

        fclose(f1); fclose(f2);
    }
    else
    {
        printf("Error: Argument count less than two.\n");
        exit(2);
    }
    exit(0);
}
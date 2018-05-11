#include <stdlib.h>
#include <stdio.h>
#include "io_file.h"

int main(int argc, char const *argv[])
{
    if (argc > 1)
    {
    	LineCount *LC = malloc(sizeof(LineCount));
    	FILE *f = fopen(argv[1], "r");
    	LC = lineCounter(f);

    	printf("Line: %d\n", LC->lines);
    	printf("Max String Length: %d\n", LC->maxStrLen);

        if ( fseek(f, 0L, SEEK_SET) == 0 )
        {
            char buffer[LC->maxStrLen];
            while(fgets(buffer, LC->maxStrLen + 2, f))
            {
                if (buffer != NULL)
                {
                    printf("%s\n", buffer);
                }
            }
        }
    }
    else
    {
    	printf("Please provide file path or name in first argument\n");
    }

    return 0;
}
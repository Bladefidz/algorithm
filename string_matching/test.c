#include <stdlib.h>
#include <stdio.h>
#include "str_match.h"

int main(int argc, char const *argv[])
{
    MatchList *ml;
    char string[] = "akumalakamaakuhila";
    char pattern[] = "aku";
    ml = naiveFindPattern(string, pattern);
    printf("Naive matched shift: ");
    while(ml != NULL) {
        printf("%d, ", ml->index);
        ml = ml->next;
    }
    printf("\n");
}
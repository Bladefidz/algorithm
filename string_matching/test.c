#include <stdlib.h>
#include <stdio.h>
#include "str_match.h"

int main(int argc, char const *argv[])
{
    MatchList *ml;
    char string[] = "aabccdeefghhiiiccdeghlabc";
    char pattern[] = "ccde";
    ml = naiveFindPattern(string, pattern, 1);
    printf("Naive matched shift: ");
    while(ml != NULL) {
        printf("%d, ", ml->index);
        ml = ml->next;
    }
    printf("\n");
    char string1[] = "abcdefghijklmnopqrstuabcdefgh";
    char pattern1[] = "abc";
    ml = naiveFindPattern(string1, pattern1, 0);
    printf("Naive matched shift unique: ");
    while(ml != NULL) {
        printf("%d, ", ml->index);
        ml = ml->next;
    }
    printf("\n");
}
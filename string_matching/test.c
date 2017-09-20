#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "str_match.h"

void printML(MatchList *ml)
{
    while(ml != NULL) {
        printf("%d, ", ml->index);
        ml = ml->next;
    }
}

int main(int argc, char const *argv[])
{
    MatchList *ml;
    printf("Naive Algorithm\n");
    ml = naiveFindPattern("aabcc", "aabccdeefghhaabccccdeghlaabcc");
    printf("Matched shift: ");
    printML(ml);
    printf("\n");
    printf("Optimized Naive Algorithm\n");
    ml = optimizedNaiveFindPattern("aabcc", "aabccdeefghhaabccccdeghlaabcc");
    printf("Matched shift: ");
    printML(ml);
    printf("\n");
    printf("Rabin-Karp Algorithm\n");
    ml = rabinKarpFindPattern("aabcc", "aabccdeefghhaabccccdeghlaabcc", 3);
    printf("Matched shift: ");
    printML(ml);
    printf("\n");
    printf("Optimized Rabin-Karp Algorithm\n");
    ml = optimizedRabinKarpFindPattern("aabcc", "aabccdeefghhaabccccdeghlaabcc", 26, 7);
    printf("Matched shift: ");
    printML(ml);
    printf("\n");
    free(ml);
}
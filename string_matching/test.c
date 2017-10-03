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
    ml = naiveStrMatch("aabcc", "aabccdeefghhaabccccdeghlaabcc");
    printf("Matched shift: ");
    printML(ml);
    printf("\n");
    printf("Optimized Naive Algorithm\n");
    ml = optimizedNaiveStrMatch("aabcc", "aabccdeefghhaabccccdeghlaabcc");
    printf("Matched shift: ");
    printML(ml);
    printf("\n");
    printf("Rabin-Karp Algorithm\n");
    ml = rabinKarpStrMatch("aabcc", "aabccdeefghhaabccccdeghlaabcc", 3);
    printf("Matched shift: ");
    printML(ml);
    printf("\n");
    printf("Optimized Rabin-Karp Algorithm\n");
    ml = optimizedRabinKarpStrMatch("aabcc", "aabccdeefghhaabccccdeghlaabcc", 26, 7);
    printf("Matched shift: ");
    printML(ml);
    printf("\n");
    printf("Deterministic Finite Automata Algorithm\n");
    ml = dfaStrMatch("aabcc", "aabccdeefghhaabccccdeghlaabcc");
    printf("Matched shift: ");
    printML(ml);
    printf("\n");
    free(ml);
}
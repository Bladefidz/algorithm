#ifndef STR_MATCH_H_
#define STR_MATCH_H_

#include <stdio.h>

/**
 * Data type declarations
 */
typedef struct MatchList {
    unsigned int index;
    struct MatchList *next;
} MatchList;


/**
 * Prototype declarations
 */
int match(char *str1, char *str2);
void pushMatch(struct MatchList** head, unsigned int index);
MatchList *naiveFindPattern(char *str, char *pattern, int repeatedChar);
MatchList *rabinKarpFindPattern(char *str, char *pattern);
MatchList *fsaFindPattern(char *str, char *pattern);
MatchList *kmpFindPattern(char *str, char *pattern);

#endif
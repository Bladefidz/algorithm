#include <stdlib.h>
#include <string.h>
#include "str_match.h"

/**
 * Check if str1 exactly match with str2
 * @param  str1 Given string
 * @param  str2 Target string to be matched
 * @return      integer
 */
int match(char *str1, char *str2)
{
    unsigned int i = 0; unsigned int m = 1;
    while (str2[i] != '\0' && str2[i] != '\0') {
        if (str2[i] != str1[i]) {
            m = 0;
            break;
        }
        i++;
    }
    return m;
}

/**
 * Find pattern using naive algorithm
 * @param  str     string
 * @param  pattern Pattern to be matched to string
 * @return         MatchList (List of matched shift)
 */
MatchList *naiveFindPattern(char *str, char *pattern)
{
    unsigned int i; unsigned int j;
    unsigned int sc = strlen(str); unsigned int pc = strlen(pattern);
    MatchList *ml = NULL; MatchList *mlc = NULL; MatchList *_ml;
    for (i = 0; i < sc; i++)
    {
        for (j = 0; j < pc; j++)
        {
            if (str[i+j] != pattern[j]) {
                break;
            }
        }
        if (j == pc) {
            if (ml == NULL) {
                _ml = (MatchList *) malloc(sizeof(MatchList));
                (*_ml).index = i;
                (*_ml).next = NULL;
                ml = mlc = _ml;
            } else {
                _ml = (MatchList *) malloc(sizeof(MatchList));
                (*_ml).index = i;
                (*_ml).next = NULL;
                (*mlc).next = _ml;
                mlc = _ml;
            }
        }
    }
    return ml;
}

MatchList *rabinKarpFindPattern(char *str, char *pattern)
{
    MatchList *ml = NULL;
    return ml;
}

MatchList *fsaFindPattern(char *str, char *pattern)
{
    MatchList *ml = NULL;
    return ml;
}

MatchList *kmpFindPattern(char *str, char *pattern)
{
    MatchList *ml = NULL;
    return ml;
}
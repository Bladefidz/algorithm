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

void pushMatch(struct MatchList** head, unsigned int index)
{
    MatchList *_ml;
    _ml = (MatchList *) malloc(sizeof(MatchList));
    (*_ml).index = index;
    (*_ml).next = NULL;
    if ((*head) != NULL)
    {
        (**head).next = _ml;
    }
    (*head) = _ml;
}

/**
 * Find pattern using naive algorithm.
 * n = strlen(str)
 * m = strlen(pattern)
 * Preprocessing time   : 0
 * Processing time      : %OMICRON((n - m + 1) m)
 * Processing time if pattern constructed by unique character: %OMICRON(n)
 * @param  str     string
 * @param  pattern Pattern to be matched to string
 * @param  unique  Indicate that pattern constructed by unique character
 * @return         MatchList (List of matched shift)
 */
MatchList *naiveFindPattern(char *str, char *pattern, int unique)
{
    unsigned int i; unsigned int j; unsigned int k;
    unsigned int n = strlen(str); unsigned int m = strlen(pattern);
    MatchList *ml = NULL; MatchList *mlc = NULL;
    if (!unique)
    {
        k = n - m;
        for (i = 0; i < n; i++)
        {
            if (i < k)
            {
                for (j = 0; j < m; j++)
                {
                    if (str[i+j] != pattern[j]) {
                        break;
                    }
                }
                if (j == m) {
                    if (ml == NULL) {
                        pushMatch(&ml, i);
                        mlc = ml;
                    } else {
                        pushMatch(&mlc, i);
                    }
                }
            }
        }
    }
    else
    {
        j = 0;
        k = m - 1;
        for (i = 0; i < n; i++)
        {
            if (j < m)
            {
                if (str[i] == pattern[j])
                {
                    if (j == k)
                    {
                        if (ml == NULL) {
                            pushMatch(&ml, i - j);
                            mlc = ml;
                        } else {
                            pushMatch(&mlc, i - j);
                        }
                        j = 0;
                    }
                    j++;
                }
                else
                {
                    if (j > 0)
                    {
                        j = 0;
                    }
                }
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
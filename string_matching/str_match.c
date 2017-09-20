#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "str_match.h"

/**
 * Push matched index in list
 * @param head  Head of matched list
 * @param index Matched index
 */
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
 * Naive algorithm to find pattern.
 * n = length of str
 * m = length of pattern)
 * Preprocessing time   : 0
 * Processing time      : %OMICRON((n - m + 1) m)
 * @param  pattern      Pattern to be matched to string
 * @param  str          string
 * @return              MatchList (List of matched shift)
 */
MatchList *naiveFindPattern(char *pattern, char *str)
{
    unsigned int i = 0; unsigned int j = 0;
    unsigned int n = strlen(str); unsigned int m = strlen(pattern);
    MatchList *ml = NULL; MatchList *mlc = NULL;
    for (; i < ((n - m) + 1); i++)
    {
        for (; j < m; j++)
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
            j = 0;
        }
    }
    return ml;
}

/**
 * Optimized naive algorithm to find pattern.
 * n = length of str
 * m = length of pattern
 * Preprocessing time   : 0
 * Processing time      : %OMICRON(n)
 * @param  pattern      Pattern to be matched to string
 * @param  str          string
 * @return              MatchList (List of matched shift)
 */
MatchList *optimizedNaiveFindPattern(char *pattern, char *str)
{
    unsigned int i = 0; unsigned int j = 0;
    unsigned int n = strlen(str); unsigned int m = strlen(pattern);
    MatchList *ml = NULL; MatchList *mlc = NULL;
    for (; i < n; i++)
    {
        if (str[i] == pattern[j])
        {
            if (j == m-1)
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
    return ml;
}

/**
 * Rabin-Karp Algorithm to Find Pattern in array
 * Treat term (char or integer) as polynomial terms. This method called as "Rolling Hash".
 * Ex: pattern "abc" in 26 alphabet = "1(26^2)+2(26^1)+3(26^0)".
 * Implementing horner rule to calculate order of polynomial.
 * https://brilliant.org/wiki/rabin-karp-algorithm/
 * Preprocessing time   : %THETA(m)
 * Processing time      : %OMICRON((n - m + 1) m)
 * @param  pattern Pattern to find
 * @param  str     String
 * @param  sigma   Number of possible atom. ex: sigma of "abbbc" is 3, "abc..z" is 26, "02..9" is 10
 * @return         MatchList (List of matched shift)
 */
MatchList *rabinKarpFindPattern(char *pattern, char *str, int sigma)
{
    MatchList *ml = NULL; MatchList *mlc = NULL;
    int i; int j;
    unsigned int n = strlen(str); unsigned int m = strlen(pattern);
    long long orderP = pattern[0] * 1; long long orderS = str[0] * 1;
    long long order = 1;
    for (i = 0; i < (m - 1); i++)
    {
        order = order * sigma;
    }
    // Do rolling hash using horner rule
    for (i = 1; i < m; i++)
    {
        orderP = pattern[i] + (sigma * orderP);
        orderS = str[i] + (sigma * orderS);
    }
    for (i = 0; i < ((n - m) + 1); i++)
    {
        if (orderS == orderP)
        {
            for (j = 0; j < m; j++)
            {
                if (str[i + j] != pattern[j])
                {
                    break;
                }
            }
            if (j == m)
            {
                if (ml == NULL) {
                    pushMatch(&ml, i);
                    mlc = ml;
                } else {
                    pushMatch(&mlc, i);
                }
            }
        }
        if (i < (n - m))
        {
            // Rehash for next window
            orderS = sigma * (orderS - str[i] * order) + str[i + m];
        }
    }
    return ml;
}

/**
 * Optimized Rabin-Karp Algorithm to Find Pattern in array
 * Improvement by using modulo to prime number to get more efficient calculation order.
 * Because, "[a]n = a + kn" equivalent to "y == b (mod n)".
 * Let b is possible length of atomic value. Ex: "12344459" has b = 6
 * Let n is common divisor or prime number.
 * Then, substitute in horner rule.
 * Resource: http://www.geeksforgeeks.org/?p=11937
 * Preprocessing time   : %THETA(m)
 * Processing time      : %OMICRON((n - m + 1) m)
 * @param  pattern Pattern to find
 * @param  str     String
 * @param  sigma   Number of possible atom. ex: sigma of abbbc is 3
 * @param  prime   Prime number
 * @return         MatchList (List of matched shift)
 */
MatchList *optimizedRabinKarpFindPattern(char *pattern, char *str, int sigma, int prime)
{
    MatchList *ml = NULL; MatchList *mlc = NULL;
    unsigned int i; unsigned int j;
    unsigned int n = strlen(str); unsigned int m = strlen(pattern);
    int order = 1;
    int orderS = 0;
    int orderP = 0;
    for (i = 0; i < (m - 1); i++)
    {
        order = (sigma * order) % prime;
    }
    for (i = 0; i < m; i++)
    {
        orderS = (sigma * orderS + str[i]) % prime;
        orderP = (sigma * orderP + pattern[i]) % prime;
    }
    for (i = 0; i <= (n - m); i++)
    {
        if (orderS == orderP)
        {
            for (j = 0; j < m; j++)
            {
                if (str[i + j] != pattern[j])
                {
                    break;
                }
            }
            if (j == m)
            {
                if (ml == NULL) {
                    pushMatch(&ml, i);
                    mlc = ml;
                } else {
                    pushMatch(&mlc, i);
                }
            }
        }
        if (i < (n - m))
        {
            orderS = (sigma * (orderS - str[i] * order) + str[i+m]) % prime;
            if (orderS < 0)
            {
                orderS = (orderS + prime);
            }
        }
    }
    return ml;
}

MatchList *fsaFindPattern(char *pattern, char *str)
{
    MatchList *ml = NULL;
    return ml;
}

MatchList *kmpFindPattern(char *pattern, char *str)
{
    MatchList *ml = NULL;
    return ml;
}
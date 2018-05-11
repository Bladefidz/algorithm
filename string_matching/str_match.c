#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "str_match.h"

#define ASCII_LEN 256

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
 * m = length of pattern
 * Preprocessing time   : 0
 * Processing time      : %OMICRON((n - m + 1) m)
 * @param  pattern      Pattern to be matched to string
 * @param  str          string
 * @return              MatchList (List of matched shift)
 */
MatchList *naiveStrMatch(char *pattern, char *str)
{
    unsigned int i = 0, j = 0;
    unsigned int n = strlen(str), m = strlen(pattern);
    MatchList *ml = NULL, *mlc = NULL;
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
MatchList *optimizedNaiveStrMatch(char *pattern, char *str)
{
    unsigned int i = 0, j = 0;
    unsigned int n = strlen(str), m = strlen(pattern);
    MatchList *ml = NULL, *mlc = NULL;
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
 * Preprocessing time   : %THETA(m)
 * Processing time      : %OMICRON((n - m + 1) m)
 * @param  pattern Pattern to find
 * @param  str     String
 * @param  sigma   Number of possible atom. ex: sigma of "abbbc" is 3, "abc..z" is 26, "02..9" is 10
 * @return         MatchList (List of matched shift)
 */
MatchList *rabinKarpStrMatch(char *pattern, char *str, int sigma)
{
    MatchList *ml = NULL, *mlc = NULL;
    int i; int j;
    unsigned int n = strlen(str), m = strlen(pattern);
    long long orderP = pattern[0] * 1, orderS = str[0] * 1, order = 1;
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
 * Preprocessing time   : %THETA(m)
 * Processing time      : %OMICRON((n - m + 1) m)
 * @param  pattern Pattern to find
 * @param  str     String
 * @param  sigma   Number of possible atom. ex: sigma of abbbc is 3
 * @param  prime   Prime number
 * @return         MatchList (List of matched shift)
 */
MatchList *optimizedRabinKarpStrMatch(char *pattern, char *str, int sigma, int prime)
{
    MatchList *ml = NULL, *mlc = NULL;
    unsigned int i, j;
    unsigned int n = strlen(str), m = strlen(pattern);
    int order = 1, orderS = 0, orderP = 0;
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

/**
 * Deterministic Finite Automaton
 * T = Text or str
 * P = Pattern
 * n = length of str
 * m = length of pattern
 * q = current state
 * Q = set of accepted states
 * delta(q, T[i]) = transition of current state (q) into another state identified by current input
 * Because we used array to achieved %OMICRON(1) to do delta(q, T[i]),
 * length of Q always equal to (m * 8 bytes)
 * Preprocessing time   : %OMICRON(m*sigma)
 * Processing time      : %THETA(n)
 * @param  pattern Pattern to find
 * @param  str     String
 * @return         MatchList (List of matched shift)
 */
MatchList *dfaStrMatch(char *pattern, char *str)
{
    MatchList *ml = NULL, *mlc;
    unsigned int n = strlen(str), m = strlen(pattern);
    unsigned int q, i, j, k;
    int Q[m][ASCII_LEN];
    for (q = 0; q < m; q++)
    { // States construction
        for (i = 0; i < ASCII_LEN; i++)
        {
            if (i == (pattern[q] * 1))
            { // Current input equal to next state
                Q[q][i] = q + 1;
                continue;
            }
            else
            {
                Q[q][i] = 0;
                for (k = q; k > 0; k--)
                { // Backtracking
                    if ((pattern[k - 1] * 1) == i)
                    { // Suffix
                        for (j = 0; j < (k - 1); j++)
                        {
                           if (pattern[j] != pattern[q - k + 1 + j])
                           {
                                break;
                           }
                        }
                        if (j == (k - 1))
                       {
                            Q[q][i] = k;
                       }
                    }
                }
            }
        }
    }
    q = 0; // Start from state 0
    for (i = 0; i < n; i++)
    {
        q = Q[q][str[i] * 1];
        if (q == m)
        {
            if (ml == NULL) {
                pushMatch(&ml, i - m + 1);
                mlc = ml;
            } else {
                pushMatch(&mlc, i - m + 1);
            }
        }
    }
    return ml;
}

/**
 * Non Deterministic Finite Automaton
 * @param  pattern Pattern to find
 * @param  str     String
 * @return         MatchList (List of matched shift)
 */
MatchList *ndfaStrMatch(char *pattern, char *str)
{
    MatchList *ml = NULL;
    return ml;
}

/**
 * Regular Expression String Match
 * @param  pattern [description]
 * @param  str     [description]
 * @return         [description]
 */
MatchList *regexStrMatch(char *pattern, char *str)
{
    MatchList *ml = NULL;
    return ml;
}

/**
 * [kmpStrMatch description]
 * @param  pattern [description]
 * @param  str     [description]
 * @return         [description]
 */
MatchList *kmpStrMatch(char *pattern, char *str)
{
    MatchList *ml = NULL;
    return ml;
}
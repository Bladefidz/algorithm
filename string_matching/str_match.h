#ifndef STR_MATCH_H_
#define STR_MATCH_H_

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
void pushMatch(struct MatchList** head, unsigned int index);
MatchList *naiveStrMatch(char *pattern, char *str);
MatchList *optimizedNaiveStrMatch(char *pattern, char *str);
MatchList *rabinKarpStrMatch(char *pattern, char *str, int sigma);
MatchList *optimizedRabinKarpStrMatch(char *pattern, char *str, int sigma, int prime);
MatchList *dfaStrMatch(char *pattern, char *str);
MatchList *ndfaStrMatch(char *pattern, char *str);
MatchList *regexStrMatch(char *pattern, char *str);
MatchList *kmpStrMatch(char *pattern, char *str);

#endif
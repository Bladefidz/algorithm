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
MatchList *naiveFindPattern(char *pattern, char *str);
MatchList *optimizedNaiveFindPattern(char *pattern, char *str);
MatchList *rabinKarpFindPattern(char *pattern, char *str, int sigma);
MatchList *optimizedRabinKarpFindPattern(char *pattern, char *str, int sigma, int prime);
MatchList *fsaFindPattern(char *pattern, char *str);
MatchList *kmpFindPattern(char *pattern, char *str);

#endif
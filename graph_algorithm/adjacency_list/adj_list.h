#ifndef ADJ_LIST_H_
#define ADJ_LIST_H_

/* Implement Node as array of pointer to achieve O(1) */
typedef struct Node
{
    unsigned int vertex;
    struct Node *next;
} Node;

void initAdjList(Node *nodes, unsigned int n);
void enqueueEdgeAdjList(Node *node, unsigned int vertex);
unsigned int dequeueEdgeAdjList(Node *node);
void transposeAdjList(Node *nodes, unsigned int n);
void printAdjList(Node *nodes, unsigned int n);

#endif
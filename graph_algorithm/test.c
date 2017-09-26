#include <stdlib.h>
#include <stdio.h>
#include "adj_matrix.h"
#include "adj_list.h"

int main(int argc, char const *argv[])
{
    // Test adjacency matrix transpose
    int m[3][3] = {{0, 1, 0}, {1, 0, 1}, {0, 1, 0}};
    printAdjMatrix(&m[0][0], 3);
    printf("Transpose:\n");
    printAdjMatrix(transposeAdjMatrix(&m[0][0], 3), 3);
    printf("\n");

    // Test adjacency list transpose
    Node *nodes = (Node *) malloc(3 * sizeof(Node));
    initAdjList(nodes, 3);
    enqueueEdgeAdjList(&nodes[0], 2);  // 1 -> 2
    enqueueEdgeAdjList(&nodes[1], 1);  // 2 -> 1
    enqueueEdgeAdjList(&nodes[1], 3);  // 2 -> 1 -> 3
    enqueueEdgeAdjList(&nodes[2], 2);  // 3 -> 2
    printAdjList(nodes, 3);
    printf("Transpose:\n");
    transposeAdjList(&nodes[0], 3);
    printAdjList(nodes, 3);

    free(nodes);

    return 0;
}
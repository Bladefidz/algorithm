#include <stdlib.h>
#include <stdio.h>
#include "adjacency_matrix/adj_matrix.h"

int main(int argc, char const *argv[])
{
    // Test adjacency matrix transpose
    int m[3][3] = {{0, 1, 0}, {1, 0, 1}, {0, 1, 0}};
    printAdjMatrix(&m[0][0], 3);
    printf("Transpose:\n");
    printAdjMatrix(transposeAdjMatrix(&m[0][0], 3), 3);

    return 0;
}
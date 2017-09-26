#include <stdio.h>
#include "adj_matrix.h"

/**
 * Print adjacency matrix
 * @param  matrix Pointer of first address of array of integer. Input should be &matrix[0][0]
 * @param  vertices Number of vertex
 */
void printAdjMatrix(int *matrix, unsigned int vertices)
{
    for (int i = 0; i < vertices; ++i)
    {
        for (int j = 0; j < vertices; ++j)
        {
            printf("%d ", *((matrix + i * vertices) + j));
        }
        printf("\n");
    }
}

/**
 * Transpose adjacency matrix where each edge that equal to 1 change into 0.
 * Running Time: %OMICRON(V*V)
 * @param  matrix Pointer of first address of array of integer. Input should be &matrix[0][0]
 * @param  vertices Number of vertex
 * @return          Pointers of integer
 */
int *transposeAdjMatrix(int *matrix, unsigned int vertices)
{
    for (int i = 0; i < vertices; ++i)
    {
        for (int j = 0; j < vertices; ++j)
        {
            if (*((matrix + i * vertices) + j) == 0) {
                *((matrix + i * vertices) + j) = 1;
            } else {
                *((matrix + i * vertices) + j) = 0;
            }
        }
    }
    return matrix;
}
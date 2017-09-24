#include "adj_matrix.h"

/**
 * Print adjacency matrix
 * @param  matrix Pointer of first address of array of integer. Input should be &matrix[0][0]
 * @param  vertex Number of vertex
 */
void printAdjMatrix(int *matrix, unsigned int vertex)
{
    for (int i = 0; i < vertex; ++i)
    {
        for (int j = 0; j < count; ++j)
        {
            printf("%d\n", *((matrix + i * vertex) + j));
        }
        printf("\n");
    }
}

/**
 * Transpose adjacency matrix where each edge that equal to 1 change into 0.
 * Running Time: %OMICRON(V*V)
 * @param  matrix Pointer of first address of array of integer. Input should be &matrix[0][0]
 * @param  vertex Number of vertex
 * @return        Pointers of integer
 */
int *transposeAdjMatrix(int *matrix, unsigned int vertex)
{
    for (int i = 0; i < vertex; ++i)
    {
        if (*(matrix + i * vertex) == p[0]) {
            for (int j = 1; j < vertex; ++j)
            {
                if (*((matrix + i * vertex) + j) == 0) {
                    *((matrix + i * vertex) + j) = 1;
                } else {
                    *((matrix + i * vertex) + j) = 1;
                }
            }
            break;
        }
    }
    return matrix;
}
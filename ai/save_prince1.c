#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/**
 * Move x until found p
 * Grid = n =  n*n, where n is odd and n+1 = '\n'
 * Moves = m = [LEFT, RIGHT, UP, DOWN]
 * Initial position = x = n*[(n/2)+1] - 1
 * Expected prince Position = p = [0, n-1, (n*n)+n-2, (n*n)-1]
 * delta = d = position transition
 */
void deterministicFindPrince(int n, char grid[101][101]) {
    char *m[] = {"LEFT", "RIGHT", "UP", "DOWN"};
    int x;
    int p[4] = {0, n-1, (n*n)+n-2, (n*n)-1};
    int pp[4][2] = {{0, 0}, {0, n-1}, {n-1, n-1}, {n-1, 0}};
    int d[4][4] = { // {transition, sum_of_transition, move, move}
        {-n-2, 2, 0, 2},
        {1, 1, 1, -1},
        {n + 1, 1, 3, -1},
        {-1, 1, 0, -1}
    };
    x = n * ( (n/2)+1 ) - 1;
    for (int i = 0; i < 4; ++i)
    {
        while (x != p[i])
        {
            for (int j = 0; j < d[i][1]; j++)
            {
                printf("%s\n", m[d[i][2+j]]);
            }
            x = x + (d[i][0]);
        }
        if (grid[pp[i][0]][pp[i][1]] == 'p') {
            break;
        }
    }
}

/**
 * Move x until found p with lowest possible step
 * Grid = n =  n*n, where n is odd and n+1 = '\n'
 * Moves = m = [LEFT, RIGHT, UP, DOWN]
 * Initial position = x = n*[(n/2)+1] - 1
 * Expected prince Position = p = [0, n-1, (n*n)+n-2, (n*n)-1]
 * delta = d = position transition
 */
void deterministicShortestPathFindPrince(int n, char grid[101][101]) {
    char *m[] = {"LEFT", "RIGHT", "UP", "DOWN"};
    int x, i, j;
    int p[4] = {0, n-1, (n*n)+n-2, (n*n)-1};
    int pp[4][2] = {{0, 0}, {0, n-1}, {n-1, n-1}, {n-1, 0}};
    int d[4][4] = { // {transition, sum_of_transition, move, move}
        {-n-2, 2, 0, 2},
        {-n, 2, 1, 2},
        {n+2, 2, 3, 1},
        {n, 2, 3, 0}
    };
    x = n * ( (n/2)+1 ) - 1;
    for (i = 0; i < 4; ++i)
    {
        if (grid[pp[i][0]][pp[i][1]] == 'p') {
            break;
        }
    }
    while (x != p[i])
    {
        for (j = 0; j < d[i][1]; j++)
        {
            printf("%s\n", m[d[i][2+j]]);
        }
        x = x + (d[i][0]);
    }
}

/**
 * https://www.hackerrank.com/challenges/saveprincess
 * Princess Peach is trapped in one of the four corners of a square grid. You are in the center of the grid and can move one step at a time in any of the four directions. Can you rescue the princess?
 * Print out the moves you will take to rescue the princess in one go. The moves must be separated by '\n', a newline. The valid moves are LEFT or RIGHT or UP or DOWN.
 * Example input:
 *     3
 *     ---
 *     -m-
 *     p--
 * Example output:
 * DOWN
 * LEFT
 * @return  int
 */
int main(void) {
    int m;
    scanf("%d", &m);
    char grid[101][101]={};
    char line[101];

    for(int i=0; i<m; i++) {
        scanf("%s", line);
        strcpy(grid[i], line);
    }
    displayPathtoPrincess(m,grid);
    return 0;
}
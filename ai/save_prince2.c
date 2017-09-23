#include <stdio.h>
#include <string.h>
#include <math.h>

void nextMove(int n, int r, int c, char grid[101][101]){
    int rp = 0, cp = 0, dr, dc, f = 0, area = 1, i, j;
    // Traveling point 0: clockwise from west-north to west-north
    int d[9][2] = {
        {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1},
        {1, 0}, {1, -1}, {0, -1}, {-1, -1}
    };
    int cV[2], nV[2];
    // Graph pair checking
    while(!f)
    {
        for (i = 0; i < 8; i++)
        {
            // Calculate current vector
            cV[0] = r + (d[i][0] * area);
            cV[1] = c + (d[i][1] * area);
            if (cV[0] >= 0 && cV[1] >= 0) {
                // Calculate next vector
                nV[0] = r + (d[i+1][0] * area);
                nV[1] = c + (d[i+1][1] * area);

                // Current vector
                if (grid[cV[0]][cV[1]] == 'p')
                {
                    rp = cV[0]; cp = cV[1];
                    f = 1;
                    break;
                }

                // Distance from current vector to next vector
                if (nV[0] - cV[0] > 0)
                {
                    for (j = cV[0]; j < nV[0]; j++)
                    {
                        if (grid[j][cV[1]] == 'p')
                        {
                            rp = j; cp = cV[1];
                            f = 1;
                            break;
                        }
                    }
                }
                if (nV[0] - cV[0] < 0)
                {
                    for (j = cV[0]; j > nV[0]; j--)
                    {
                        if (grid[j][cV[1]] == 'p')
                        {
                            rp = j; cp = cV[1];
                            f = 1;
                            break;
                        }
                    }
                }
                if (nV[1] - cV[1] > 0)
                {
                    for (j = cV[1]; j < nV[1]; j++)
                    {
                        if (grid[cV[0]][j] == 'p')
                        {
                            rp = cV[0]; cp = j;
                            f = 1;
                            break;
                        }
                    }
                }
                if (nV[1] - cV[1] < 0)
                {
                    for (j = cV[1]; j > nV[1]; j--)
                    {
                        if (grid[cV[0]][j] == 'p')
                        {
                            rp = cV[0]; cp = j;
                            f = 1;
                            break;
                        }
                    }
                }

                if (i < 7) {
                    // Next vector
                    if (grid[nV[0]][nV[1]] == 'p')
                    {
                        rp = nV[0]; cp = nV[1];
                        f = 1;
                        break;
                    }
                }
            }
        }
        area++;
    }

    if (f == 1)
    {
        // Calculate distance
        if (rp > r)
        {
            dr = rp - r;
        }
        else
        {
            dr = r - rp;
        }
        if (cp > c)
        {
            dc = cp - c;
        }
        else
        {
            dc = c - cp;
        }
        // Movement decision
        if (dr + dc > 1)
        {
            if (dr == dc)
            {
/**
p   x   x   x   p
x   p   ?   p   x
x   ?   m   ?   x
x   p   ?   p   x
p   x   x   x   p
 */
                if (r > rp)
                {
                    printf("UP\n");
                }
                else
                {
                    printf("DOWN\n");
                }
            }
            else
            {
/**
x   p   p   p   x
p   x   ?   x   p
p   ?   m   ?   p
p   x   ?   x   p
x   p   p   p   x
 */
                if (dr > dc)
                {
                    if (r < rp)
                    {
                        printf("DOWN\n");
                    }
                    else
                    {
                        printf("UP\n");
                    }
                }
                else if (dr < dc)
                {
                    if (c < cp)
                    {
                        printf("RIGHT\n");
                    }
                    else
                    {
                        printf("LEFT\n");
                    }
                }
            }
        }
        else if (dr + dc == 1)
        {
/**
x   x   x   x   x
x   x   p   x   x
x   p   m   p   x
x   x   p   x   x
x   x   x   x   x
 */
            if (r != rp)
            {
                if (r > rp)
                {
                    printf("UP\n");
                }
                else
                {
                    printf("DOWN\n");
                }
            }
            if (c != cp)
            {
                if (c > cp)
                {
                    printf("LEFT\n");
                }
                else
                {
                    printf("RIGHT\n");
                }
            }
        }
    }
}

/**
 * https://www.hackerrank.com/challenges/saveprincess2
 * @return  [description]
 */
int main(void) {
    // n = Grid area
    // r c = row and column of bot
    int n, r, c;

    scanf("%d", &n);
    scanf("%d", &r); scanf("%d", &c);

   char grid[101][101];

    for(int i=0; i<n; i++) {
        scanf("%s[^\n]%*c", grid[i]);
    }

    nextMove(n, r, c, grid);
    return 0;
}
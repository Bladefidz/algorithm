#include <stdio.h>
#include <string.h>
#include <math.h>
void dfs(int r, int c, int pacman_r, int pacman_c, int food_r, int food_c, char grid[r][c]){
    //logic here
}

int main(void) {
    int r, c;
    int pacman_r, pacman_c;
    int food_r, food_c;
    scanf( "%d %d", &pacman_r, &pacman_c);
    scanf( "%d %d", &food_r, &food_c);
    scanf( "%d %d", &r, &c);
    char grid[r][c];

    for( int i=0; i<r; i++) {
        scanf("%s[^\\n]%*c", grid[i]);
    }
    dfs( r, c, pacman_r, pacman_c, food_r, food_c, grid);
    return 0;
}
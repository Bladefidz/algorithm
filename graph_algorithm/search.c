#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graph_search.h"

/* Global variable declarations */
Graph *myGraph;

/* Function declarations */

void ui()
{

}

/**
 * Main test program
 * @param  argc Argument count: Should be equal or more than 3
 * @param  argv Argument variable: "./test algorithm input_file"
 * @return      [description]
 */
int main(int argc, char const *argv[])
{
    Graph *paths = (Graph *) malloc(VERTEX * sizeof(Graph));

    if (argc > 2)
    {

    } else {
        if (argc == 1)
        {
            if (strcmp(argv[1]) == "-h")
            {
                printf("Demonstrate Graph Searching Algorithm\n\n");
                printf("Format\n\t<algorithm> <input>\n");
                printf("Algorithm\n\tbfs: Breath First Search\n");
                printf("\tdfs: Depth First Search\n");
                printf("Input\n\tpath/to/file.txt\n");
                printf("\tLook test.txt for input file format\n");
                printf("\nExample: ./test bfs test.txt");
            }
        }
        else
        {
            printf("Error: Type -h for help.\n");
            exit(0);
        }
    }

    return 0;
}
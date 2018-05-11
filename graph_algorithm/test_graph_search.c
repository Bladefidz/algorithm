#include <stdlib.h>
#include <stdio.h>
#include "graph/graph.h"
#include "graph_search/graph_search.h"

int main(int argc, char const *argv[])
{
	unsigned int i;

	initGraphFromFile("dummy_path.txt");

	int *path = dfs(getGraph(), 0);
	printf("Depth First Search\n");
	printf("Path of vertex A: ");
	i = 0;
	while (path[i] != -1) {
		printf("-%s ", getVertexName(path[i]));
		i++;
	}

	printf("\n");

	int *path1 = bfs(getGraph(), 0);
	printf("Breath First Search\n");
	printf("Path of vertex A: ");
	i = 0;
	while (path1[i] != -1) {
		printf("-%s ", getVertexName(path1[i]));
		i++;
	}

	printf("\n");

	return 0;
}

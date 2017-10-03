#include <stdlib.h>
#include <stdio.h>
#include "graph/graph.h"

int main(int argc, char const *argv[])
{
	initGraphFromFile("dummy_path.txt");
	printf("Vertex\n");
	printVertex();
	printf("Graph\n");
	printGraph();

	return 0;
}

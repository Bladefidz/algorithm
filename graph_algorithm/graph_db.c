#include <stdlib.h>
#include <stdio.h>
#include "graph/graph.h"

int main(int argc, char const *argv[])
{
	initGraphFromFile("dummy_path.txt");
	printAllVertex();

	return 0;
}
#include <stdlib.h>
#include <stdio.h>
#include "graph_search.h"

/* Global declarations */
struct VertexChild
{
    unsigned int id;
    unsigned int weight;
    VertexChild *next;
} VertexChild;
struct Vertex
{
    char *name;
    unsigned int childCount;
    VertexChild **childs;
} Vertex;
struct Graph
{
    unsigned int vertexCount;
    unsigned int edgeCount;
    Vertex *vertex;
} Graph;
struct Path
{
    unsigned int vertex;
    unsigned int pre;
    Path *next;
} Path;

/* Function Declarations */
void allocateVertexNames(unsigned int n)
{

}
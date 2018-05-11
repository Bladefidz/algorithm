#ifndef GRAPH_H_
#define GRAPH_H_

/* Type definitions */
typedef struct _VertexChild VertexChild;
typedef struct _Vertex Vertex;
typedef struct _Graph Graph;

/* Global variable declarations */
struct _VertexChild
{
    unsigned int id;
    unsigned int weight;
    VertexChild *next;
};
struct _Vertex
{
    unsigned int id;
    unsigned int childCount;
    VertexChild *child;
};
struct _Graph
{
    unsigned int vertexCount;
    unsigned int edgeCount;
    Vertex **vertex;
};

/* Prototype declarations */
void setError(char *message);
char *getError();
void printError();
void allocateGrap(unsigned int n);
char *getVertexName(unsigned int id);
int getVertexId(char *name);
void printVertex();
int addEdge(unsigned int parentId, unsigned int childId);
int initGraphFromFile(char *f);
void printGraph();
Graph *getGraph();
int destroyGraph(Graph *G);
int destroyVertexNames(Graph *G);

#endif

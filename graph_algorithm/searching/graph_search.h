#ifndef GRAPH_SEARCH_H_
#define GRAPH_SEARCH_H_

/* Global variable declarations */
typedef struct VertexChild;
typedef struct Vertex;
typedef struct Graph;
typedef struct Path;

/* Private variable declarations */
static char *VERTEX_NAMES;

/* Prototype declarations */
void allocateVertexNames(unsigned int n);
void allocateGraph(Graph *G, unsigned int v);
void allocatePath(Path *P);
Graph *parseFileToGraph(char *f);
unsigned int getVertexIndex(char *vname);
void pushPath(Path *P, unsigned int v);
unsigned int popPath(Path *P);
void enqueuePath(Path *P, unsigned int v);
unsigned int dequeuePath(PATH *P);
PATH *dfs(Graph *G, unsigned int s, unsigned int w);
PATH *bfs(Graph *G, unsigned int s, unsigned int w);
void printGraph(Graph *G, unsigned int v);
void printPath(Graph *G, unsigned int v);
int destroyGraph(Graph *G);
int destroyVertexNames(Graph *G);
int destroyPath(Path *p);

#endif
#ifndef GRAPH_H_
#define GRAPH__H_

/* Type definitions */
typedef struct _VertexChild VertexChild;
typedef struct _Vertex Vertex;
typedef struct _Graph Graph;
typedef struct _Path Path;

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
struct _Path
{
    unsigned int vertex;
    unsigned int pre;
    Path *next;
};

/* Prototype declarations */
void setError(char *message);
char *getError();
void printError();
void allocateGrap(unsigned int n);
char *getVertex(unsigned int id);
int getVertexId(char *name);
void printVertex();
int addEdge(unsigned int parentId, unsigned int childId);
int initGraphFromFile(char *f);
void printGraph();
void allocatePath(Path *P);
void pushPath(Path *P, unsigned int v);
unsigned int popPath(Path *P);
void enqueuePath(Path *P, unsigned int v);
unsigned int dequeuePath(Path *P);
void printPath(Graph *G, unsigned int v);
int destroyGraph(Graph *G);
int destroyVertexNames(Graph *G);
int destroyPath(Path *p);

#endif

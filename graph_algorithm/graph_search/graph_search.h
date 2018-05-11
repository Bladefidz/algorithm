#ifndef GRAPH_SEARCH_H_
#define GRAPH_SEARCH_H_
#include "../graph/graph.h"

/* Type definitions */
typedef struct _Path Path;

/* Global variable declarations */

/* Prototype declarations */
unsigned int prepareVisitList(Graph *G);
void setVisited(unsigned int vertedId);
unsigned int isVisited(unsigned int vertedId);
int *dfs(Graph *G, unsigned int from);
int *bfs(Graph *G, unsigned int from);
int *dls(Graph *G, unsigned int from);

#endif

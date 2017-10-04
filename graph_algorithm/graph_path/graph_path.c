#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graph_path.h"
#include "../graph/graph.h"

/* Typed definitions */
typedef struct _VQueue VQueue;

/* Struct declarations */
struct _VQueue
{
    unsigned int vertex;
    VQueue *next;
};

/* Private variable declaration */
static unsigned int *_VISITLIST = NULL;
static int *_PATH = NULL;
static unsigned int _PC;
static VQueue *VQ = NULL, *VQ_Curr = NULL;
static unsigned int VQ_COUNT;

/* Private function */
static unsigned int _prepareVisitList(Graph *G);
static void _setVisited(unsigned int vertedId);
static unsigned int _isVisited(unsigned int vertedId);
static void _preparePath();
static unsigned int _prepareVQ();
static void _enqueueVertex(unsigned int vertexId);
static unsigned int _dequeueVertex();
static void _dfs(Graph *G, unsigned int from);

/* Function Declaration */
/**
 * Prepare visit list as an array
 * @param  G Graph object
 * @return   Integer
 */
static unsigned int _prepareVisitList(Graph *G)
{
    if (_VISITLIST != NULL)
    {
        free(_VISITLIST);
    }
    _VISITLIST = malloc(sizeof(unsigned int) * G->vertexCount);
    if (_VISITLIST)
    {
        for (unsigned int i = 0; i < G->vertexCount; i++) {
            _VISITLIST[i] = 0;
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * Set corresponding vertex id as visited
 * @param vertedId Vertex id
 */
static void _setVisited(unsigned int vertedId)
{
    _VISITLIST[vertedId] = 1;
}

/**
 * Check if vertex has been visited
 * @param  vertedId Vertex id
 * @return          0 or 1
 */
static unsigned int _isVisited(unsigned int vertedId)
{
    return _VISITLIST[vertedId];
}

/**
 * Prepare path
 */
static void _preparePath(Graph *G)
{
    if (_PATH != NULL)
    {
        free(_PATH);
    }
    _PATH = malloc(sizeof(_PATH) * G->vertexCount);
    _PC = 0;
}

/**
 * Depth Fisrt Search interface
 * @param  G    Graph object
 * @param  from Start of vertex id or node
 * @return      Array of int
 */
int *dfs(Graph *G, unsigned int from)
{
	_prepareVisitList(G);
    _preparePath(G);
    _dfs(G, from);
    _PATH[_PC + 1] = -1;
    return _PATH;
}

/**
 * Recursive Depth First Search
 * @param G    Graph object
 * @param from Vertex or node
 */
static void _dfs(Graph *G, unsigned int from)
{
    _setVisited(from);
    VertexChild *vc;
    vc = G->vertex[from]->child;
    for (unsigned int i = 0; i < G->vertex[from]->childCount; i++)
    {
        if (!_isVisited(vc->id))
        {
            // Push vertex into path
            _PATH[_PC] = vc->id;
            _PC++;

            _dfs(G, vc->id);
        }
        vc = vc->next;
    }
}

/**
 * Prepare Vertex Queue
 * @return   Success: 1, else: 0
 */
static unsigned int _prepareVQ()
{
    if (VQ != NULL)
    {
        free(VQ);
    }
    VQ = malloc(sizeof(VQ));
    VQ->next = NULL;
    if (VQ)
    {
        VQ_COUNT = 0;
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * Enqueue vertex to queue
 * @param vertexId Vertex id
 */
static void _enqueueVertex(unsigned int vertexId)
{
    if (VQ_COUNT == 0)
    {
        VQ->vertex = vertexId;
        VQ_Curr = VQ;
        VQ_COUNT++;
    }
    else
    {
        VQueue *new = malloc(sizeof(VQueue));
        if (new)
        {
            new->vertex = vertexId;
            new->next = NULL;
            VQ_Curr->next = new;
            VQ_Curr = VQ_Curr->next;
            VQ_COUNT++;
        }
    }
}

/**
 * Dequeue vertex from queue
 * @return vertex id
 */
static unsigned int _dequeueVertex()
{
    unsigned int v = (*VQ).vertex;
    if ((*VQ).next != NULL && VQ_COUNT > 0)
    {
        VQueue *next = (*VQ).next;
        VQ = next;
    }
    VQ_COUNT--;
    return v;
}

/**
 * Breath First Search Interface
 * @param  G    Graph object
 * @param  from Start of vertex id or node
 * @return      Array of int
 */
int *bfs(Graph *G, unsigned int from)
{
    _prepareVisitList(G);
    _preparePath(G);
    _prepareVQ();

    _enqueueVertex(from);
    _setVisited(from);

    unsigned int v;
    VertexChild *vc;

    while (VQ_COUNT > 0)
    {
        v = _dequeueVertex();
        vc = G->vertex[v]->child;
        for (unsigned int i = 0; i < G->vertex[v]->childCount; i++)
        {
            if (!_isVisited(vc->id))
            {
                _enqueueVertex(vc->id);
                _setVisited(vc->id);

                // Push vertex into path
                _PATH[_PC] = vc->id;
                _PC++;
            }
            vc = vc->next;
        }
    }
    _PATH[_PC] = -1;
    return _PATH;
}

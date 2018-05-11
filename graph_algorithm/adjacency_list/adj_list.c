#include <stdlib.h>
#include <stdio.h>
#include "adj_list.h"

/**
 * Initialize adjacency list
 * @param nodes Start address of adjacency list
 * @param n     Number of vertex
 */
void initAdjList(Node *nodes, unsigned int n)
{
    for (int i = 0; i < n; i++)
    {
        (*(nodes + i)).vertex = 0;
        (*(nodes + i)).next = NULL;
    }
}

/**
 * Enqueue new edge to another vertex from given vertex.
 * Implementing priority queue.
 * @param node   Address of vertex node
 * @param vertex Vertex to add into node
 */
void enqueueEdgeAdjList(Node *node, unsigned int vertex)
{
    if ((*node).vertex == 0)
    {
        (*node).vertex = vertex;
    }
    else
    {
        Node *current = node, *next;
        while (current != NULL)
        {
            if ((*current).vertex == vertex)
            { // Already registered
                break;
            }
            else if ((*current).vertex > vertex)
            { // Found bigger child, insert vertex before it
                next = current;

                Node *new = (Node *) malloc(sizeof(Node));
                (*new).vertex = vertex;
                (*new).next = NULL;

                current = new;
                (*current).next = next;

                break;
            }
            else
            {
                if ((*current).next == NULL)
                {
                    Node *new = (Node *) malloc(sizeof(Node));
                    (*new).vertex = vertex;
                    (*new).next = NULL;

                    (*current).next = new;
                }
                current = (*current).next; // Only to make sure to stop loop
            }
        }
    }
}

/**
 * Dequeue corresponding vertex of given vertex node
 * @param node   Address of vertex node
 * @param vertex Vertex to add into node
 */
unsigned int dequeueEdgeAdjList(Node *node)
{
    unsigned int v = (*node).vertex;
    if ((*node).next != NULL)
    {
        Node *next = (*node).next;
        *node = *next;
    }
    else
    {
        if ((*node).vertex != 0)
            (*node).vertex = 0;
    }
    return v;
}

/**
 * Transpose adjacency list
 * @param nodes [description]
 * @param n     [description]
 */
void transposeAdjList(Node *nodes, unsigned int n)
{
    unsigned int j, k, l, tmp[n];

    for (unsigned int i = 0; i < n; i++)
    {
        j = 0;
        do
        {
            tmp[j] = dequeueEdgeAdjList(&nodes[i]);
            ++j;
        } while (tmp[j-1] != 0);
        l = 0;
        for (k = 0; k < n; k++)
        {
            if (((k+1) != tmp[l]) || (tmp[l] == 0))
            {
                enqueueEdgeAdjList(&nodes[i], k+1);
            }
            else
            {
                if (l < j)
                    l++;
            }
        }
    }
}

/**
 * Print adjacency list
 * @param nodes Start address of adjacency list
 * @param n     Number of vertex
 */
void printAdjList(Node *nodes, unsigned int n)
{
    Node *current;
    for (int i = 0; i < n; ++i)
    {
        printf("%d -> ", i + 1);
        current = nodes++;
        while(current != NULL)
        {
            printf("%d ", (*current).vertex);
            current = (*current).next;
            if (current != NULL)
                printf(" -> ");
        }
        printf("\n");
    }
}

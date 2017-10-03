#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graph.h"
#include "../../io/io_file.h"

/* Private variable declarations */
static char *LAST_ERROR_MESSAGE = NULL;
static char **VERTEX_NAMES;
static char *FILE_PATH;
static Graph *G = NULL;

/* Function Declarations */
/**
 * Get last error message
 * @return string
 */
char *getError()
{
    return LAST_ERROR_MESSAGE;
}

/**
 * Print last error message
 */
void printError()
{
    if (LAST_ERROR_MESSAGE != NULL)
    {
        printf("%s\n", LAST_ERROR_MESSAGE);
    }
}

/**
 * Allocate a new Graph and Vertex.
 * Allocate vertex as array as id-name pair.
 * Allocate empty Graph with zero edge.
 * @param n Number of vertex
 */
void allocateGrap(unsigned int n)
{
    VERTEX_NAMES = malloc(sizeof(VERTEX_NAMES) * n);
    if (VERTEX_NAMES)
    {
        G = malloc(sizeof(Graph));
        G->vertexCount = n;
        G->edgeCount = 0;
        G->vertex = malloc(sizeof(Vertex) * n);
    }
}

/**
 * Get specific Vertex by id
 * @param  id Id of a vertex
 * @return    string
 */
char *getVertex(unsigned int id)
{
    if (G != NULL)
    {
        if (id < G->vertexCount)
            return VERTEX_NAMES[id];
    }
    return NULL;
}

/**
 * Get vertex id by name
 * @param  name vertex name
 * @return      id
 */
int getVertexId(char *name)
{
    if (G != NULL)
    {
        unsigned int i;
        for (i = 0; i < G->vertexCount; i++) {
            if (strcmp(name, VERTEX_NAMES[i]) == 0)
            {
                break;
            }
        }
        if (i < G->vertexCount)
        {
            return i;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
}

/**
 * Print all Vertex names
 */
void printVertex()
{
    if (G != NULL)
    {
        for (int i = 0; i < G->vertexCount; i++)
        {
            printf("%d: %s\n", i, *(VERTEX_NAMES + i));
        }
    }
    printf("\n");
}

/**
 * Parse text file and build a new Graph
 * @param f File path
 */
int initGraphFromFile(char *f)
{
    FILE *file;
    int success = 1;
    unsigned int i0, i1;
    char *buffer, *tmp;

    file = fopen(f, "r");
    if (file)
    {
        FILE_PATH = malloc(sizeof(char) * (strlen(f) + 1));
        strcpy(FILE_PATH, f);

        // Get line information and allocate buffer
        LineCount *LC = malloc(sizeof(LineCount));
        LC = lineCounter(file);
        allocateGrap(LC->lines);
        buffer = malloc(sizeof(buffer) * LC->maxStrLen);

        // Set pointer to read at first line of file
        if ( fseek(file, 0L, SEEK_SET) == 0 )
        {  // Scan for vertex
            // The scanning length should be max_length + 2 because carriage return (\r\t)
            for (int i = 0; i < LC->lines; i++)
            {
                fgets(buffer, LC->maxStrLen + 2, file);

                if (buffer != NULL)
                {
                    i0 = i1 = 0;

                    while(buffer[i1] != EOF)
                    {  // Parse buffered string
                        if (buffer[i1] == '[')
                        {
                            i0 = i1 + 1;
                        }
                        else
                        {
                            if (buffer[i1] == ']')
                            {
                                VERTEX_NAMES[i] = malloc(sizeof(VERTEX_NAMES) * (i1 - i0));
                                G->vertex[i] = malloc(sizeof(Vertex) * (i1 - i0));
                                G->vertex[i]->id = i;
                                G->vertex[i]->childCount = 0;
                                G->vertex[i]->child = NULL;
                                for (int j = 0; j < (i1 - i0); j++)
                                {
                                    VERTEX_NAMES[i][j] = buffer[i0 + j];
                                }
                                i0 = 0;
                                break;
                            }
                        }
                        i1++;
                    }
                }
            }
        }
        else
        {  // Can not reset pointer back to first line
            success = 0;
        }

        // Set pointer to read at first line of file
        if ( fseek(file, 0L, SEEK_SET) == 0 )
        {  // Scan for edge
            VertexChild *curV, *newV;
            for (int i = 0; i < LC->lines; i++)
            {
                fgets(buffer, LC->maxStrLen + 2, file);

                if (buffer != NULL)
                {
                    i0 = i1 = 0;

                    while(i1 < LC->maxStrLen)
                    {  // Parse buffered string
                        if (buffer[i1] == '-')
                        { // Get edges
                            i1 += 1;
                            i0 = i1;

                            // Get next - or newline
                            while (buffer[i1] != '-' && buffer[i1] != '\n') {
                                i1++;
                            }

                            tmp = malloc(sizeof(char) * (i1 - i0));
                            for (int j = 0; j < (i1 - i0); j++)
                            { // Get vertex's name
                                tmp[j] = buffer[i0 + j];
                            }

                            // push edge
                            newV = malloc(sizeof(VertexChild));
                            newV->id = getVertexId(tmp);
                            newV->weight = 1;
                            newV->next = NULL;
                            if (G->vertex[i]->child == NULL)
                            {
                                G->vertex[i]->child = newV;
                                curV = G->vertex[i]->child;
                            }
                            else
                            {
                                curV->next = newV;
                                curV = newV;
                            }

                            G->vertex[i]->childCount += 1;

                            free(tmp);
                            i1 -= 1;
                        }
                        i1++;
                    }
                }
            }
        }
        else
        {  // Can not reset pointer back to first line
            success = 0;
        }

        free(buffer);
        fclose(file);
    }
    else
    {  // Can not open file
        success = 0;
    }

    if (success)
    {
        return 1;
    }
    else
    {  // There is a line that written in not acceptable format
        return 0;
    }
}

/**
 * Print allocated Graph
 */
void printGraph()
{
    if (G != NULL)
    {
        VertexChild *vc;
        for (int i = 0; i < G->vertexCount; i++)
        {
            printf("|%d|", G->vertex[i]->id);
            vc = G->vertex[i]->child;
            for (int j = 0; j < G->vertex[i]->childCount; j++) {
                printf(" -> %d", vc->id);
                vc = vc->next;
            }
            printf("\n");
        }
    }
}

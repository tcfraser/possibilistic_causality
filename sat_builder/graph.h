#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

typedef char Vertex;

typedef struct {
    Vertex parent;
    Vertex child;
} Edge;

typedef struct {
    uint numVertices;
    uint numEdges;
    Vertex *vertices;
    Edge *edges;
} Graph;

typedef struct {
    uint length;
    uint *indices;
} PaIdx; /* ParentIndex */

typedef struct {
    PaIdx root;
    PaIdx nonRoot;
} PaPaIdx; /* PartitionedParentIndex */

typedef struct {
    uint length;
    PaPaIdx *data;
} PaPaGraph;

void freeGraph(Graph *g);
void freePaPaGraph(PaPaGraph *papag);
void printGraph(FILE *fp, Graph *g);
void printPaPaGraph(FILE *fp, PaPaGraph *papag);
bool scanFileUntil(FILE *fp, char c);
Graph *readGraph(FILE *fp);
PaPaGraph *gToPaPaG(Graph *g);

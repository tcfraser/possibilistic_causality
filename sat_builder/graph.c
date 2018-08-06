#include "graph.h"
#include "fileio.h"

void freeGraph(Graph *g) {
    free(g->vertices);
    free(g->edges);
    free(g);
}

void freePaPaGraph(PaPaGraph *papag) {
    for (uint i = 0; i < papag->length; ++i) {
        free(papag->data[i].root.indices);
        free(papag->data[i].nonRoot.indices);
    }
    free(papag->data);
    free(papag);
}

void printGraph(FILE *fp, Graph *g) {
    uint length = 2 + g->numVertices + (g->numVertices - 1) + 1 + 2 + 4 * g->numEdges + (g->numEdges - 1) + 1; /* +1 for null terminator. */
    uint position = 0;
    uint i;
    Edge *curEdge;
    char *buffer = malloc(length * sizeof(char));

    /* writing vertices */
    position += sprintf(buffer + position, "(");
    for (i = 0; i < g->numVertices; ++i) {
        if (i > 0) {
            position += sprintf(buffer + position, ",");
        }
        position += sprintf(buffer + position, "%c", g->vertices[i]);
    }
    position += sprintf(buffer + position, ") ");

    position += sprintf(buffer + position, "[");

    for (i = 0; i < g->numEdges; ++i)
    {
        if (i > 0) {
            position += sprintf(buffer + position, ",");
        }
        curEdge = g->edges + i;
        position += sprintf(buffer + position, "%c->%c", curEdge->parent, curEdge->child);
    }
    position += sprintf(buffer + position, "]");
    buffer[position] = '\0';
    fprintf(fp, "%s\n", buffer);
    free(buffer);
}

void printPaPaGraph(FILE *fp, PaPaGraph *papag) {
    for (uint i = 0; i < papag->length; ++i) {
        fprintf(fp, "[");
        for (uint j = 0; j < papag->data[i].root.length; ++j) {
            if (j > 0) { fprintf(fp, " "); }
            fprintf(fp, "%u", papag->data[i].root.indices[j]);
        }
        fprintf(fp, "|");
        for (uint k = 0; k < papag->data[i].nonRoot.length; ++k) {
            if (k > 0) { fprintf(fp, " "); }
            fprintf(fp, "%u", papag->data[i].nonRoot.indices[k]);
        }
        fprintf(fp, "]");
    }
    fprintf(fp, "\n");
}

int readEdge(FILE *fp, Edge *e) {
    if (scanFileUntil(fp, ' ')) {
        return fscanf(fp, "%c %c", &(e->parent), &(e->child));
    } else {
        return 0;
    }
}


uint _mergeVertex(Vertex *list, Vertex v, uint count) {

    bool isMember = false;
    for (uint i = 0; i < count; ++i) {
        if (list[i] == v) {
            isMember = true;
            break;
        }
    }
    if (!isMember) {
        list[count] = v;
        return count + 1;
    } else {
        return count;
    }
}

Vertex *_vertices(Graph *g) {

    Vertex *ret = malloc(g->numVertices * sizeof(Vertex));

    uint curCount = 0;
    for (uint i = 0; i < g->numEdges; ++i) {
        curCount = _mergeVertex(ret, g->edges[i].child, curCount);
        curCount = _mergeVertex(ret, g->edges[i].parent, curCount);
        if (curCount == g->numVertices) {
            break;
        }
    }
    return ret;

}

Graph *readGraph(FILE *fp) {

    if (!scanFileUntil(fp, 'g')) {

        /* if no marker is found before hitting the end of the file */
        /* returning a NULL pointer to indicate that the file contains proper graph object */
        return NULL;
    }
    Graph *g = malloc(sizeof(Graph));

    int matchedValues = fscanf(fp, " %u %u", &(g->numVertices), &(g->numEdges));

    if (matchedValues != 2) {
        return NULL;
    }

    g->edges = malloc(g->numEdges * sizeof(Edge));

    Edge *e_ptr = g->edges; /* need this copy to safely free memory latter */

    while (e_ptr - g->edges < g->numEdges) {
        readEdge(fp, e_ptr++);
    }

    g->vertices = _vertices(g);

    return g;
}

bool isRoot(Vertex v, Graph *g) {

    uint i;
    for (i = 0; i < g->numEdges; ++i)
    {
        if ( (g->edges[i]).child == v ) {
            return false;
        }
    }
    return true;
}

uint intPosnOfVertex(Graph *g, Vertex v) {
    uint i;
    for (i = 0; i < g->numVertices; ++i) {
        if (g->vertices[i] == v) {
            break;
        }
    }
    return i;
}

PaIdx *computePaIdx(Graph *g, Vertex v) {

    PaIdx *paidx = malloc(sizeof(PaIdx));

    paidx->length = 0;
    uint i;
    for (i = 0; i < g->numEdges; ++i) {
        if ( g->edges[i].child == v ) {
            paidx->length += 1;
        }
    }

    if (paidx->length > 0) {
        paidx->indices = malloc(paidx->length * sizeof(uint));
        uint *ptr = paidx->indices;

        uint j;
        for (i = 0; i < g->numEdges; ++i) {
            if ( g->edges[i].child == v ) {
                *(ptr++) = intPosnOfVertex(g, g->edges[i].parent);
            }
        }
    } else {
        paidx->indices = NULL;
    }

    return paidx;
}

void computePaPaIdx(PaPaIdx *papaidx_cast, PaIdx *paidx, bool *rootMask, uint *indices) {

    PaIdx *r  = &(papaidx_cast->root);
    PaIdx *nr = &(papaidx_cast->nonRoot);

    uint i;
    for (i = 0; i < paidx->length; ++i) {
        r->length += rootMask[paidx->indices[i]];
    }

    nr->length = paidx->length - r->length;

    nr->indices = malloc(nr->length * sizeof(uint));
    r->indices  = malloc(r->length  * sizeof(uint));

    uint *idx_pair[2] = {nr->indices, r->indices};

    for (i = 0; i < paidx->length; ++i) {
        *(idx_pair[rootMask[paidx->indices[i]]]++) = indices[paidx->indices[i]];
    }
}

PaPaGraph *gToPaPaG(Graph *g) {

    PaPaGraph *papag = malloc(sizeof(PaPaGraph));

    papag->length = 0;

    /* 1) the rootMask; rootMask[i] is true if the i-th variable is root. */
    bool *rootMask = malloc(g->numVertices * sizeof(int));
    /* 2) the integer indices to be assigned independently to the root and non-root variables */
    uint *indices  = malloc(g->numVertices * sizeof(uint));
    uint counter[2] = {0, 0}; /* non-root, root counter */
    uint i;
    for (i = 0; i < g->numVertices; ++i) {
        rootMask[i] = isRoot(g->vertices[i], g);
        indices[i]  = counter[rootMask[i]]++;
        papag->length += 1 - rootMask[i];
    }

    // for (i = 0; i < g->numVertices; ++i) {
    //     printf("rootMask[%i] = %i, indices[%i] = %u\n", i, rootMask[i], i, indices[i]);
    // }
    // printf("%u\n", papag->length);

    PaIdx *paidx;
    papag->data = malloc(papag->length * sizeof(PaPaIdx));
    PaPaIdx *tmp_papag = papag->data;
    for (i = 0; i < g->numVertices; ++i) {
        if (!rootMask[i]) {
            paidx = computePaIdx(g, g->vertices[i]);

            // printf("[%i]", i);
            // for (uint k = 0; k < paidx->length; ++k) {
            //     printf("%u", paidx->indices[k]);
            // }
            // printf("\n");

            computePaPaIdx(tmp_papag++, paidx, rootMask, indices);
            free(paidx);
        } else {
            /* root vertices have no parents by definition; no need to compute anything */
            continue;
        }
    }

    free(rootMask);
    free(indices);

    return papag;
}

int _graph(void) {

    /*
        The stdout buffer doesn't flush unless printing a newline character.
        Need to figure out printf calls in scope get removed from stdout on exit scope.
        This must be a bug of some sort.
    */
    // setbuf(stdout, NULL);
    printf("[graph.c]\n");
    printf("sizeof(char)   = %u\n", sizeof(char));
    printf("sizeof(Vertex) = %u\n", sizeof(Vertex));
    printf("sizeof(Edge)   = %u\n", sizeof(Edge));
    printf("sizeof(Graph)  = %u\n", sizeof(Graph));

    FILE* fp = fopen("file_io/example_graphs_file_2.txt", "r");

    if (fp == NULL) {
        printf("%s\n", "fopen returned a NULL pointer, exiting now.");
        return 1;
    }

    Graph *g = readGraph(fp);

    printGraph(stdout, g);

    PaPaGraph *papag = gToPaPaG(g);

    freeGraph(g);

    return 0;
}
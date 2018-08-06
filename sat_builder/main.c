#include <stdio.h>

#include "cnf_file_io.h"

void debugPointerAddress(char *str, void *ptr) {
    printf("Address of %s is %p (%u).\n", str, ptr, ptr);
}

int main() {
    printf("Printing the sizes of various types.\n");
    printf("sizeof(char)     = %i\n", sizeof(char));
    printf("sizeof(int)      = %i\n", sizeof(int));
    printf("sizeof(Clause)   = %i\n", sizeof(Clause));
    printf("sizeof(Clauses)  = %i\n", sizeof(Clauses));
    printf("sizeof(Support)  = %i\n", sizeof(Support));
    printf("sizeof(Vertex)   = %i\n", sizeof(Vertex));
    printf("sizeof(Edge)     = %i\n", sizeof(Edge));
    printf("sizeof(Graph)    = %i\n", sizeof(Graph));
    printf("sizeof(PaIdx)    = %i\n", sizeof(PaIdx));
    printf("sizeof(PaPaIdx)  = %i\n", sizeof(PaPaIdx));
    printf("sizeof(PaPaGraph)= %i\n", sizeof(PaPaGraph));
    debugPointerAddress("NULL", NULL);
    char *null_ptr_test;
    debugPointerAddress("null_ptr_test", null_ptr_test);

    FILE* fp  = fopen("file_io/test.in", "w+");
    FILE* gfp = fopen("file_io/tetrahedron_graph.txt", "r");
    FILE* sfp = fopen("file_io/example_supports_file_3.txt", "r");
    printf("fp = %p, gfp = %p, sfp = %p\n");

    if (fp == NULL || gfp == NULL || sfp == NULL) {
        printf("error :: a file pointer was null\n");
        fclose(fp);
        fclose(gfp);
        fclose(sfp);
        return 1;
    }

    /* Reading graph data. */
    Graph *g = readGraph(gfp);
    printf("Example Graph:\n");
    printGraph(stdout, g);

    /* Reading support data. */
    Support *s = readSupport(sfp);

    if (s == NULL) {
        printf("error :: readSupport returned NULL pointer \n");
        return 1;
    }
    printf("Example Support:\n");
    printSupport(stdout, s);

    uint order = 3;

    pipeline(g, s, order, fp);

    freeGraph(g);
    freeSupport(s);
    return 0;
}
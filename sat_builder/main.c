#include <stdio.h>

#include "cnf_file_io.h"

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

    FILE* fp = fopen("file_io/test.in", "w+");

    if (fp == NULL) {
        printf("%s\n", "File returned NULL, exiting.");
        return 1;
    }

    char *null_ptr_test;
    debugPointerAddress("null_ptr_test", null_ptr_test);

    FILE* gfp = fopen("file_io/example_graphs_file_3.txt", "r");
    if (gfp == NULL) { return 1; }

    Graph *g = readGraph(gfp);

    printf("Example Graph:\n");
    printGraph(stdout, g);

    FILE* sfp = fopen("file_io/example_supports_file_2.txt", "r");

    if (sfp == NULL) { return 1; }

    Support *s = readSupport(sfp);

    printf("Example Support:\n");
    printSupport(stdout, s);

    uint order = 1;

    pipeline(g, s, order, fp);

    freeGraph(g);
    freeSupport(s);

    // uint radices[] = {2, 2, 2, 4, 4, 4, 4};
    // uint digits[] = {0, 0, 0, 0, 0, 0, 0};

    // MixedRadix exampleMR = { .length = 7, .radices = radices};

    int fpCloseResponse = fclose(fp);

    if (fpCloseResponse == EOF) {
        printf("%s\n", "File closed unsuccessfully.");
    } else {
        printf("%s\n", "File closed successfully.");
    }
    printf("fpCloseResponse = %i\n", fpCloseResponse);

    return 0;
}
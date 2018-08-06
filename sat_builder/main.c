#include <stdio.h>

#include "cnf_file_io.h"

void debugPointerAddress(char *str, void *ptr) {
    printf("Address of %s is %p (%u).\n", str, ptr, ptr);
}

int main() {

    int exitCode = 0;

    printf("Printing the sizes of various types.\n");
    printf("sizeof(char)       = %i\n", sizeof(char));
    printf("sizeof(int)        = %i\n", sizeof(int));
    printf("sizeof(Clause)     = %i\n", sizeof(Clause));
    printf("sizeof(Clauses)    = %i\n", sizeof(Clauses));
    printf("sizeof(Support)    = %i\n", sizeof(Support));
    printf("sizeof(Vertex)     = %i\n", sizeof(Vertex));
    printf("sizeof(Edge)       = %i\n", sizeof(Edge));
    printf("sizeof(Graph)      = %i\n", sizeof(Graph));
    printf("sizeof(PaIdx)      = %i\n", sizeof(PaIdx));
    printf("sizeof(PaPaIdx)    = %i\n", sizeof(PaPaIdx));
    printf("sizeof(PaPaGraph)  = %i\n", sizeof(PaPaGraph));

    debugPointerAddress("NULL", NULL);
    char *null_ptr_test;
    debugPointerAddress("null_ptr_test", null_ptr_test);

    printf("areStringsEqual(\"cat\", \"dog\") = %s\n", areStringsEqual("cat", "dog") ? "true" : "false");
    printf("areStringsEqual(\"cat\", \"car\") = %s\n", areStringsEqual("cat", "car") ? "true" : "false");
    printf("areStringsEqual(\"cat\", \"cat\") = %s\n", areStringsEqual("cat", "cat") ? "true" : "false");
    printf("areStringsEqual(\"cat\", \"category\") = %s\n", areStringsEqual("cat", "category") ? "true" : "false");

    FILE* fpi = fopen("file_io/test.in", "w+");
    FILE* fpo = fopen("file_io/test.out", "r");
    FILE* gfp = fopen("file_io/tetrahedron_graph.txt", "r");
    FILE* sfp = fopen("file_io/example_supports_file_3.txt", "r");
    printf("fp = %p, fpo = %p, gfp = %p, sfp = %p\n",
        fpi, fpo, gfp, sfp);

    if (fpi != NULL && fpo != NULL && gfp != NULL && sfp != NULL) {
        /* Reading graph data. */
        Graph *g = readGraph(gfp);
        printf("Example Graph:\n");
        printGraph(stdout, g);

        /* Reading support data. */
        Support *s = readSupport(sfp);

        if (s == NULL) {
            printf("error :: readSupport returned NULL pointer \n");
            exitCode = 1;
        } else {
            printf("Example Support:\n");
            printSupport(stdout, s);

            writeCNFForGraphSupportOrder(g, s, 3, fpi);

            freeSupport(s);
        }
        freeGraph(g);

        printf("%i\n", getCNFOutStatus(fpo));
    } else {
        printf("error :: a file pointer was null\n");
        exitCode = 1;
    }

    fclose(fpi);
    fclose(fpo);
    fclose(gfp);
    fclose(sfp);

    return exitCode;
}
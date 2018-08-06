#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "mixedradix.h"
#include "graph.h"
#include "support.h"

typedef struct {
    /* .length is the total number of literals in the clause */
    uint length;
    /* .literals is a pointer to the literals within this clause */
    int *literals;
} Clause;

typedef struct {
    /* .length is the total number of clauses */
    uint length;
    /* .data is a pointer to the clauses */
    Clause *data;
} Clauses;

void writeCNFForGraphSupportOrder(Graph *g, Support *s, uint order, FILE *fp);
bool areStringsEqual(char *a, char *b);
int getCNFOutStatus(FILE *fp);

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

void debugPointerAddress(char *str, void *ptr);
void freeClauses(Clauses *clauses);
void writeClause(Clause *clause, FILE *fp);
void writeClauses(Clauses *clauses, FILE *fp);
void pipeline(Graph *g, Support *s, uint order, FILE *fp);

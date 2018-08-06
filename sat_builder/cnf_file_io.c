#include "cnf_file_io.h"

void freeClauses(Clauses *clauses) {
    uint c; /* Clause index. */
    for(c = 0; c < clauses->length; ++c) {
        free((clauses->data)[c].literals);
    }
    free(clauses->data);
    free(clauses);
}

void _writeClause(Clause *clause, FILE *fp) {
    for(uint l = 0; l < clause->length; ++l) {
        fprintf(fp, "%i ", clause->literals[l]);
    }
    fprintf(fp, "0\n"); /* DIMACS format is to end clauses with terminating zero character. */
}

void _writeClauses(Clauses *clauses, FILE *fp) {
    for(uint c = 0; c < clauses->length; ++c) {
        _writeClause(clauses->data + c, fp);
    }
}

int _computeBVIndex(uint v, uint *event, uint *world, uint order, PaPaGraph *papag, uint offset) {
    assert(0 <= v < papag->length);
    int idx = offset;
    uint k;

    // printf("(%u) ", v);
    // printf(" e(");
    // for (k = 0; k < papag->length; ++k) { printf("%u", event[k]); }
    // printf(") ");
    // printf("\n");

    /* need to increment the index value depending on all of the boolean variables for vertices less than v */
    for (k = 0; k < v; ++k) {
        idx += power(order, papag->data[k].root.length) * power(2, papag->data[k].nonRoot.length);
    }

    uint worldFactor = 0;
    for (k = 0; k < papag->data[v].root.length; ++k) {
        worldFactor += power(order, k) * world[papag->data[v].root.indices[k]];
    }
    for (k = 0; k < papag->data[v].nonRoot.length; ++k) {
        worldFactor += power(2, k)     * event[papag->data[v].nonRoot.indices[k]];
    }
    idx += worldFactor;

    if (event[v] == 0) {
        idx = -idx;
    } else {
        /* event[v] == 1, leave idx as-is */
    }

    return idx;
}

/* Computes the world-support variable index. */
uint _computeBVHelperIndex(uint w, uint e, uint numWorlds, uint numEvents, uint offset) {
    return w * numEvents + e + offset;
}

Clauses *_createHelperClauses(uint numWorlds, uint numEvents, uint offset) {

    Clauses *wec = (Clauses *) malloc(sizeof(Clauses));

    wec->length = numWorlds + numEvents;

    wec->data = (Clause *) malloc(wec->length * sizeof(Clause));

    uint w; /* The world index. */
    uint e; /* The event index. */
    uint c; /* The clause counter. */
    c = 0;

    int *literals;
    for (w = 0; w < numWorlds; ++w) {

        literals = (int *) malloc(numEvents * sizeof(int));

        for (e = 0; e < numEvents; ++e) {
            literals[e] = _computeBVHelperIndex(w, e, numWorlds, numEvents, offset);
        }

        (wec->data)[c].length = numEvents;
        (wec->data)[c].literals = literals;
        c += 1;
    }

    for (e = 0; e < numEvents; ++e) {

        literals = (int *) malloc(numWorlds * sizeof(int));

        for (w = 0; w < numWorlds; ++w) {
            literals[w] = _computeBVHelperIndex(w, e, numWorlds, numEvents, offset);
        }

        (wec->data)[c].length = numWorlds;
        (wec->data)[c].literals = literals;
        c += 1;
    }

    return wec;
}

void writeCNFForGraphSupportOrder(Graph *g, Support *s, uint order, FILE *fp) {

    PaPaGraph *papag = gToPaPaG(g);

    uint v, e, w;
    int z, evw;
    uint numEvents = s->numEvents;
    uint numNonRootVariables = papag->length;
    uint numRootVariables = g->numVertices - numNonRootVariables;

    assert(numNonRootVariables == s->numVariables);

    uint numWorlds = power(order, numRootVariables);
    uint numWorldEventPairs = numWorlds * numEvents;

    fprintf(fp, "c ==============================================\n");
    fprintf(fp, "c numWorlds :: %u, numEvents :: %u, order :: %u.\n",
        numWorlds, numEvents, order);
    fprintf(fp, "c Support :: ");
    printSupport(fp, s);
    fprintf(fp, "c Graph :: ");
    printGraph(fp, g);
    fprintf(fp, "c PaPaGraph :: ");
    printPaPaGraph(fp, papag);

    /* Variable counting, indexing and offsetting. */
    uint bVariableAcc = 1;

    for (v = 0; v < numNonRootVariables; ++v)
    {
        fprintf(fp, "c vars {%5u ... ", bVariableAcc);
        fprintf(fp, "%5u} are the responses of nonRoot variable %u.\n",
            (bVariableAcc += power(order, papag->data[v].root.length) * power(2, papag->data[v].nonRoot.length))-1, v);
    }
    uint bHelperOffet = bVariableAcc;
    fprintf(fp, "c vars {%5u ... ", bVariableAcc); // need to do this because order of valuation of  arguments is undefined.
    fprintf(fp, "%5u} are event*world helper variables.\n", (bVariableAcc += numWorldEventPairs)-1);
    uint numVariablesTotal = bVariableAcc-1;
    /* */

    // /* Constructing and Writing the world-event clauses. */
    // fprintf(fp, "c Creating clauses for world-event consistency. \n");
    // fprintf(fp, "c X_z = { 1 if world w supports event e \nc       { 0 if world w does not support event e: \n");
    // fprintf(fp, "c [for each] world (w) [there exists] an event (e) such that (w) supports (e). \n");
    // fprintf(fp, "c [for each] event (e) [there exists] a world (w) such that (w) supports (e). \n");

    /* Clause counting, indexing and offsetting. */

    uint numClausesTotal = numWorlds + numEvents + numWorldEventPairs * (1 + numNonRootVariables);

    /* */
    fprintf(fp, "c ==============================================\n");
    fprintf(fp, "p cnf %u %u\n", numVariablesTotal, numClausesTotal);
    /* Constructing and Writing the variable-world-event clauses. */

    uint *world = calloc(numRootVariables, sizeof(uint));
    uint *event;

    Clause  *pZClause  = malloc(sizeof(Clause)); // Clause where Z is positive
    pZClause->length   = numNonRootVariables + 1;
    pZClause->literals = malloc(pZClause->length * sizeof(int));

    Clauses *nZClauses = malloc(sizeof(Clauses)); // Clauses where Z is negative
    nZClauses->length  = numNonRootVariables;
    nZClauses->data    = malloc(nZClauses->length * sizeof(Clause));
    for (v = 0; v < numNonRootVariables; ++ v) {
        nZClauses->data[v].length   = 2;
        nZClauses->data[v].literals = malloc(nZClauses->data[v].length * sizeof(int));
    }

    for (w = 0; w < numWorlds; ++w)
    {
        event = s->data; /* reset */
        for (e = 0; e < numEvents; ++e)
        {
            z = _computeBVHelperIndex(w, e, numWorlds, numEvents, bHelperOffet);

            pZClause->literals[0] = z;
            for (v = 0; v < numNonRootVariables; ++ v)
            {
                evw = _computeBVIndex(v, event, world, order, papag, 1);

                nZClauses->data[v].literals[1] = evw;
                nZClauses->data[v].literals[0] = -z;

                pZClause->literals[v+1] = -evw;
            }

            _writeClause(pZClause, fp);
            _writeClauses(nZClauses, fp);

            event += numNonRootVariables; /* increment pointer by the number of variables. see support.c for reason why */
        }
        homoIncrementDigits(order, numRootVariables, world);
    }

    /*  */
    Clauses *wec = _createHelperClauses(numWorlds, numEvents, bHelperOffet);
    _writeClauses(wec, fp);

    freeClauses(wec);
    free(pZClause->literals);
    free(pZClause);
    freeClauses(nZClauses);
    free(world);
    freePaPaGraph(papag);
}

int getCNFOutStatus(FILE *fp) {
    char cur = fgetc(fp);

    if (cur == EOF) {
        return -1; /* file ends */
    }
    if (cur != 's') {
        if (scanFileUntil(fp, '\n')) {
            return getCNFOutStatus(fp);
        } else {
            return -1; /* no more newlines */
        }
    } else {
        char status[14]; /* UNSATISFIABLE is 13 characters long plus '\0' char */
        if (fscanf(fp, " %s", &status) != 1) {
            return -1; /* improper format. line does not start with "p ..." */
        } else {
            if (areStringsEqual(status, "SATISFIABLE")) {
                return 1;
            } else if (areStringsEqual(status, "UNSATISFIABLE")) {
                return 0;
            } else {
                return -1; /* improper format. p is followed by something other than SATISFIABLE or UNSATISFIABLE */
            }
        }
    }
}

bool areStringsEqual(char *a, char *b)
{
    int i;
    bool result = true;
    for(i=0; a[i]!='\0' || b[i]!='\0'; i++) {
        // printf("%c, %c\n", a[i], b[i]);
        if(a[i] != b[i]) {
            result = false;
            break;
        }
    }
    return result;
}
#include <stdio.h>

typedef struct {
    /* .length is the total number of literals in the clause */
    uint length;
    /* .literals is a pointer to the literals within this clause */
    int *literals;
} Clause;

typedef struct {
    /* .length is the total number of clauses */
    uint length;
    /* .clauses is a pointer to the clauses */
    Clause *clauses;
} Clauses;

typedef struct {
    uint numWorlds;
    uint numSupports;
} WorldSupportContext;

/* Clauses are pointers to arrays of *signed* integers terminated by a zero. */

int writeClause(int *clause, FILE *fp) {
    for(; *clause; ++clause) { /* for loop terminates when *clause hits the zero terminating int. */
        fprintf(fp, "%i ", *clause);
    }
    fprintf(fp, "0\n"); /* DIMACS format is to end clauses with terminating zero character. */
}

int writeClauses(int **clauses, uint numClauses, FILE *fp) {

    for (int i = 0; i < numClauses; ++i)
    {
        writeClause(clauses[i], fp);
        // if (i != numClauses - 1)
        // {
        //     fputc('\n', fp);
        // }
    }
}

int **generateWorldSupportClauses(WorldSupportContext wsc) {

    int clauses[wsc.numSupports][wsc.numWorlds];

    return clauses;
}


int writeWorldSupportClauses(WorldSupportContext wsc, FILE *fp) {

    uint z;
    uint numClauses = wsc.numWorlds;
    uint currentClauseIndex = 0;
    fprintf(fp, "c numWorlds = %u, numSupports = %u, numClauses = %u.\n", wsc.numWorlds, wsc.numSupports, numClauses);
    int *clauses[numClauses];

    /*
        Boolean representation:

        z = i * wsc.numSupports + j + 1;

        X_z = { 1 if world i supports event j
              { 0 if world i does not support event j
    */

    fprintf(fp, "c [for each] world (w) [there exists] an event (e) such that (w) supports (e). \n");
    for (uint i = 0; i < wsc.numWorlds; ++i) {

        int worldClause[wsc.numSupports + 1];
        worldClause[0] = -3;
        worldClause[1] = -4;
        worldClause[2] = -5;
        worldClause[wsc.numSupports] = 0; /* Ensures that each clause is zero terminated. */

        for (uint j = 0; j < wsc.numSupports; ++j) {
            z = i * wsc.numSupports + j + 1;
            // printf("z assigned to %u\n", z);
            worldClause[j] = z;
        }
        writeClause(worldClause, fp);

        clauses[currentClauseIndex] = worldClause; /* i.e. &(worldClause[0]) */
        printf("currentClauseIndex = %u\n", currentClauseIndex);
        currentClauseIndex += 1;
    }

    // writeClauses(clauses, numClauses, fp);

}

int main()
{
    /* code */

    FILE* fp = fopen("file_io/test.in", "w+");

    if (fp == NULL) {
        printf("%s\n", "File returned NULL, exiting.");
        return 1;
    }

    // Clause clauses[numClauses];

    int literals[] = {3, 6, -3, -110, 1430, 0}; /* Notice the 0-terminated literal list. */
    int *exampleClause1 = literals;
    int *exampleClause2 = &(literals[0]);

    // printf("%i\n", exampleClause1 - exampleClause2);
    // printf("%i\n", *literals);

    int numExampleClauses = 10;
    int *exampleClauses[] = {
        (int[]){47,-42,38,-57,-56,68,67,64,63,0},
        (int[]){46,44,40,-57,56,-50,-65,-64,-62,-61,0},
        (int[]){47,46,38,56,-54,-63,60,-59,0},
        (int[]){46,-44,56,-54,52,-50,-62,-61,-60,0},
        (int[]){-42,38,-52,50,48,62,61,0},
        (int[]){-46,42,40,-56,52,50,-65,-64,-62,0},
        (int[]){-47,-44,42,56,-52,69,-65,-63,62,-59,0},
        (int[]){-46,-44,-42,-52,50,-69,-66,58,0},
        (int[]){-10,-8,7,-18,-16,36,34,-33,0},
        (int[]){-4,-14,-27,-22,0}
    };

    for (int i = 0; i < numExampleClauses; ++i)
    {
        printf("Address of exampleClauses[%i] is (%u) %p.\n", i, exampleClauses[i], exampleClauses[i]);
    }

    writeClause(exampleClause1, fp);
    fputc('\n', fp);
    writeClauses(exampleClauses, numExampleClauses, fp);

    WorldSupportContext wsc = {.numWorlds = 1, .numSupports = 8};

    writeWorldSupportClauses(wsc, fp);

    // clauses[0] = {47,-42,38,-57,-56,68,67,64,63,0};
    // clauses[1] = {46,44,40,-57,56,-50,-65,-64,-62,-61,0};
    // clauses[2] = {47,46,38,56,-54,-63,60,-59,0};
    // clauses[3] = {46,-44,56,-54,52,-50,-62,-61,-60,0};
    // clauses[4] = {-42,38,-52,50,48,62,61,0};
    // clauses[5] = {-46,42,40,-56,52,50,-65,-64,-62,0};
    // clauses[6] = {-47,-44,42,56,-52,69,-65,-63,62,-59,0};
    // clauses[7] = {-46,-44,-42,-52,50,-69,-66,58,0};
    // clauses[8] = {-10,-8,7,-18,-16,36,34,-33,0};
    // clauses[9] = {-4,-14,-27,-22,0};


    // writeClauses(clauses, fp);

    // writeClause(exampleClause, fp);

    int fpCloseResponse = fclose(fp);

    if (fpCloseResponse == EOF) {
        printf("%s\n", "File closed unsuccessfully.");
    } else {
        printf("%s\n", "File closed successfully.");
    }
    printf("fpCloseResponse = %i\n", fpCloseResponse);

    return 0;
}
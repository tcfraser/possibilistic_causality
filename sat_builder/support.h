#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    uint numVariables;
    uint numEvents;
    uint *data;
} Support;

void freeSupport(Support *s);

Support *readSupport(FILE *fp);
void printSupport(FILE *fp, Support *s);
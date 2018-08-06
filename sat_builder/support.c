#include "support.h"
#include "fileio.h"

void freeSupport(Support *s) {
    free(s->data);
    free(s);
}

void printSupport(FILE *fp, Support *s) {

    for (uint e = 0; e < s->numEvents; ++e) {
        fprintf(fp, "[");
        for (uint v = 0; v < s->numVariables; ++v) {
            if (v > 0) { fprintf(fp, " "); }
            fprintf(fp, "%u", s->data[ e * s->numVariables + v ]);
        }
        fprintf(fp, "]");
    }
    fprintf(fp, "\n");
}

Support *readSupport(FILE *fp) {
    /*
     * Returns the next Support found in the file pointer *fp.
     * Returns NULL pointer if no supports are found in *fp,
     * or if support is improperly formatted.
     */

    if (!scanFileUntil(fp, 's')) {

        /* if no marker is found before hitting the end of the file */
        /* returning a NULL pointer to indicate that the file contains proper support object */
        return NULL;
    }
    Support *s = malloc(sizeof(Support));

    int matchedValues = fscanf(fp, " %u %u", &(s->numVariables), &(s->numEvents));

    if (matchedValues != 2) {
        return NULL;
    }

    uint length = s->numEvents * s->numVariables;
    s->data = malloc(length * sizeof(uint));

    uint *d_ptr = s->data; /* need this copy to safely free memory latter */

    while (d_ptr - s->data < length) {
        if (scanFileUntil(fp, ' ')) {
            *(d_ptr++) = fgetc(fp) - '0';
        } else {
            return NULL;
        }
    }

    return s;
}

int _support(void) {

    FILE* fp = fopen("file_io/example_supports_file.txt", "r");

    if (fp == NULL) {
        printf("%s\n", "fopen returned a NULL pointer, exiting now.");
        return 1;
    }

    Support *s = readSupport(fp);

    // for (int i = 0; i < s->numVariables * s->numEvents; ++i)
    // {
    //     printf("%u", s->data[i]);
    // }
    // printf("\n");

    freeSupport(s);

    return 0;
}
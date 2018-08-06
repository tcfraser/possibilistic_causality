#include "fileio.h"

bool scanFileUntil(FILE *fp, char c) {
    int marker;
    do {
        marker = fgetc(fp);
        if (marker == EOF) {
            return false;
        }
    } while (marker != c);
    return true;
}
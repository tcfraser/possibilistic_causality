#include <stdio.h>

typedef struct {
    uint length;
    int* radices;
} MixedRadix;

// void setHomogenousMR(MixedRadix *mr, uint length, uint order);

uint power(uint a, uint b);

uint mrMax(MixedRadix *mr);

uint digitsToInt(MixedRadix *mr, uint *digits);

void *incrementDigits(MixedRadix *mr, uint *digits);

void *homoIncrementDigits(uint order, uint length, uint *digits);

uint homoDigitsToInt(uint order, uint length, uint *digits);

void printDigits(MixedRadix *mr, uint *digits);
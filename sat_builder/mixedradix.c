#include <stdio.h>
#include "mixedradix.h"

// void setHomogenousMR(MixedRadix *mr, uint length, uint order) {
//     mr->length = length;
//     if (length > 0) {
//         mr->radices = malloc(length * sizeof(uint));
//         for (uint i = 0; i < length; ++i) {
//             mr->radices[i] = order;
//         }
//     } else {
//         mr->radices = NULL;
//     }
// }

uint power(uint a, uint b) {
    if (b == 0) {
        return 1;
    } else {
        return a * power(a, b-1);
    }
}

uint mrMax(MixedRadix *mr) {
    uint val = 1;
    uint i;
    for (i = 0; i < mr->length; ++i) {
        val *= mr->radices[i];
    }
    return val;
}

uint _digitsToInt(uint position, uint *radices, uint *digits) {
    if (position == 0) {
        return 0;
    }
    position -= 1;
    return digits[position] + radices[position] * _digitsToInt(position, radices, digits);
}

uint digitsToInt(MixedRadix *mr, uint *digits) {
    return _digitsToInt(mr->length, mr->radices, digits);
}

void *_incrementDigits(int position, uint length, uint *radices, uint *digits) {
    if (position < 0) {
        _incrementDigits(length-1, length, radices, digits);
    } else if (digits[position] < radices[position]-1) {
        digits[position] += 1;
    } else {
        digits[position] = 0;
        if (position > 0) {
            _incrementDigits(--position, length, radices, digits);
        }
    }
}

void *incrementDigits(MixedRadix *mr, uint *digits) {
    _incrementDigits(mr->length-1, mr->length, mr->radices, digits);
}

void *_homoIncrementDigits(int position, uint order, uint length, uint *digits) {
    if (position < 0) {
        _homoIncrementDigits(length-1, order, length, digits);
    } else if (digits[position] < order-1) {
        digits[position] += 1;
    } else {
        digits[position] = 0;
        if (position > 0) {
            _homoIncrementDigits(--position, order, length, digits);
        }
    }
}

void *homoIncrementDigits(uint order, uint length, uint *digits) {
    _homoIncrementDigits(length-1, order, length, digits);
}

uint homoDigitsToInt(uint order, uint length, uint *digits) {
    uint ret = 0;
    for (uint j = 0; j < length; ++j) {
        ret += power(order, length - 1 - j) * digits[length - 1 - j];
    }
    return ret;
}

void printDigits(MixedRadix *mr, uint *digits) {
    uint i;
    for (i = 0; i < mr->length; ++i) {
        if (i > 0) { printf("|"); }
        printf("%u_%u", digits[i], mr->radices[i]);
    }
    printf("\n");
}

int test(void)
{
    uint radices[] = {3, 4, 3, 5, 3};
    uint digits[] = {2, 1, 2, 0, 1};

    MixedRadix exampleMR = { .length = 5, .radices = radices};

    printDigits(&exampleMR, digits);
    printf("digitsToInt(&exampleMR, {2, 1, 2, 0, 1}) = %u\n", digitsToInt(&exampleMR, digits));
    printf("mrMax(&exampleMR) = %u\n", mrMax(&exampleMR));

    uint incrementtest[] = {0, 0, 0, 0, 0};

    for (int i = 0; i < mrMax(&exampleMR); ++i) {
        printDigits(&exampleMR, incrementtest);
        incrementDigits(&exampleMR, incrementtest);
    }

    return 0;
}
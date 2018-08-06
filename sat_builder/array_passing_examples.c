#include <stdio.h>

typedef struct {
    uint length;
    char* list_ptr;
} charList;

/* function declaration */
double getAverage(int arr[], int size);

void printCharList(charList arg);

void uintPointerPrinter(uint* arg);

void charPointerPrinter(char* arg);

int main () {

    int i;
    char array[] = "don't panic";
    char* ptr = array;

    /* array traversal */
    for (i = 0; i < sizeof(array); ++i)
        printf("%c ", array[i]);

    printf("\n");

    /* pointer traversal */
    for (; *ptr; ++ptr)
        printf("%c ", *ptr);
    printf("\n");

    char chars[] = "Hello World";
    printf("\n");

    uint numbers[] = {0,1,2,3,4,5,6,7,8,9};

    uintPointerPrinter(numbers);

    printf("\n");
    charPointerPrinter(chars);

    charList x = {10, chars};

    charList y;

    y.length = 10;
    y.list_ptr = "Hello World";

    /* an int array with 5 elements */
    int balance[5] = {1000, 2, 3, 17, 50};
    double avg;

    /* pass pointer to the array as an argument */
    avg = getAverage( balance, 5 ) ;

    /* output the returned value */
    printf("\n");
    printf("\n");
    printf( "Average value is: %f ", avg );

    return 0;
}

void printCharList(charList arg) {

}

void uintPointerPrinter(uint* arg) {
    printf("%i", *arg);
    for (; *arg; ++arg) {
        printf("%i", *arg);
    }
}

void charPointerPrinter(char* arg) {

    for (; *arg; ++arg) {
        printf("%c", *arg);
    }
    printf("%c", *arg);
}

double getAverage(int arr[], int size) {

    int i;
    double avg;
    double sum = 0;

    for (i = 0; i < size; ++i) {
      sum += arr[i];
    }

    avg = sum / size;

    return avg;
}
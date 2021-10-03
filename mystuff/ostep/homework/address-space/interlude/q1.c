#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/*
    gcc -o out.bin q1.c -Wall
    ./out.bin

*/

int main() {

    int * p;
    p = NULL;

    // seg fault (illegal memory access)
    printf("%d", *p);
}
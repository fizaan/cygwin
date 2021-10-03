#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/*
    gcc -o out.bin q5.c -Wall
    ./out.bin

*/

int main() {

    int * data = (int *) malloc(sizeof(int) * 100);
    data[100] = 55;

    // This program will compile and run fine, but is not correct.
    // 100 index is out of bounds
}
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/*
    gcc -o out.bin q7.c -Wall
    ./out.bin

*/

int main() {

    int * data = (int *) malloc(sizeof(int) * 100);
    
    free(data + 50); // noting happens. Yes we need tools like valgrind to find the actual problem
}
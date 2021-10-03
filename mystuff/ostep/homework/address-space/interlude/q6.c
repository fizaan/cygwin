#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/*
    gcc -o out.bin q6.c -Wall
    ./out.bin

*/

int main() {

    int * data = (int *) malloc(sizeof(int) * 100);
    
    free(data);

    printf("%d", data[78]); // prints 0
}
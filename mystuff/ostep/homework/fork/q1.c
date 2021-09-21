#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

/*
    gcc -o out.bin q1.c -Wall
    ./out.bin

    Child and parent will both have their OWN copies of xCopy and can only
    
    modify their own copies.

    ./out.bin
    Child: 100, 20
    Parent: 100, 10

*/

int main() {

    int xCopy = 100;

    int pid = fork();

    if( pid == 0) {
        
        // child

        printf("Child: %d", xCopy);

        xCopy = 20;

        printf(", %d\n", xCopy);

    } else {
        // parent

        // let child run first
        wait(NULL);

        printf("Parent: %d", xCopy);

        xCopy = 10;

        printf(", %d\n", xCopy);
    }
    
}
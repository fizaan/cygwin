#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// gcc -o out.bin whatthef.c -Wall

//  https://www.youtube.com/watch?v=xVSPv-9x3gk

// "Fork makes two identical copies of the address spaces; 
// one for the child and one for the parent” - that is incorrect. 
// If that were true we would see THREE pids, not two.

int main() {
    // parent pid
    printf("Parent: %d\n", getpid());

    int newp = fork();

    if(newp < 0 ) {
        //error
    }
    else if(newp == 0) { // child
        printf("Child: %d\n", getpid());
    }
    else {  // parent
        printf("Parent: %d\n", getpid());
    }
}
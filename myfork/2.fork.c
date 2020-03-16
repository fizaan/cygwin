#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

/*
    gcc -o 2.bin 2.fork.c -Wall
    ./1.bin

    $ ./2.bin
    I am a child [1753] of [1752]
    I am a child [1754] of [1752]
    I am a child [1755] of [1752]
    I am a child [1756] of [1752]
    I am the parent: [1752]
    I am a child [1757] of [1752]

*/

int main(int argc, char *argv[]) {
    for(int i = 0; i < 5; i++ )
        if(fork() == 0)  { // child
            printf("I am a child [%d] of [%d]\n", getpid(), getppid());
            exit(0);
        }
    
    printf("I am the parent: [%d]\n", getpid());
    for(int i = 0; i < 5; i++)
        wait(NULL);
}
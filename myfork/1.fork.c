#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

/*
    gcc -o 1.bin 1.fork.c -Wall
    ./1.bin

    $ ./1.bin
    I am the main program [1738]. I am useless. Don't give me any name
    I am the parent (clone of main) [1738]
    I am the child [1739] of [1738]

*/

int main(int argc, char *argv[]) {
    printf("I am the main program [%d]. I am useless. Don't give me any name\n",getpid());
    int pid = fork();
    if(pid == 0) {
        printf("I am the child [%d] of [%d]\n", getpid(), getppid());
        exit(0);
    }
    else { // you don't need the else but whatever.
        printf("I am the parent (clone of main) [%d]\n", getpid());
        wait(NULL);
    }    
}
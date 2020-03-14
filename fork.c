#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    printf("hello world (pid:%d)\n", (int) getpid());
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) // child (new process)
        printf("hello, I am child (pid:%d)\n", (int) getpid());
    else // parent goes down this path (original process)
        printf("hello, I am parent of %d (pid:%d)\n",rc, (int) getpid());
    return 0;
}

/*
    cygwin 64 terminal window:
    $ cd /cygdrive/c/Users/Alifa/Desktop/cygwin64-workspace
    $ gcc -o 1.runfork.bin fork.c
    
    $ ./1.runfork.bin
    hello world (pid:1831)
    hello, I am parent of 1832 (pid:1831)
    hello, I am child (pid:1832)


*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

/*
    File descriptor table:

    0 = STDIN_FILENO
    1 = STDOUT_FILENO
    2 = STDERR_FILENO
    3 = unsued
    4 = unused

    pipe:
    + ---------------------------- +
    + wR (child)         (parent) r  +
    + ---------------------------- +
*/

#define wR  1 //write end of pipe (child sends)
#define r  0  //read end of pipe (parent receives)
#define psize  2 //pipe size

void check(int ret);

int main(int argc, char * argv[]) {
    int p[psize];
    pipe(p);
    int sender = fork();
    check(sender);

    if(sender == 0) { //child (sender) writes to pipe
        //first close unwanted read end of pipe:
        check(close(p[r]));
        //notice the white space. We need it for scanf.
        char hi[] = "Hithere ";
        check(close(STDOUT_FILENO));
        check(dup(p[wR]));
        printf("%d:%s",getpid(),hi);
        fflush(stdout);
        //close pipe's write end after use
        check(close(p[wR]));
        exit(0);
    }

    //in the parent (receiver) reads from pipe
    //and prints to parent stdout

    //first close unwated write end of pipe:
    check(close(p[wR]));
    char received[20];
    check(close(STDIN_FILENO));
    check(dup(p[r]));
    scanf("%s",received);
    fflush(stdin);
    printf("1: Parent[%d] received: %s\n", getpid(), received); 
    //close pipe's read end after use
    check(close(p[r]));
    
    wait(NULL);
    return 0;


}

void check(int ret) {
    if(ret<0) {
        printf("Error");
        exit(-1);
    }
}


/*

EBC+Alifa@ON34C02750570 /cygdrive/c/Users/Alifa/Desktop/cygwin64-workspace/mystuff/tasks/2.SimplePipe
$ gcc -o 3.bin 3.scanFpipe.c -Wall

EBC+Alifa@ON34C02750570 /cygdrive/c/Users/Alifa/Desktop/cygwin64-workspace/mystuff/tasks/2.SimplePipe
$ ./3.bin
1: Parent[644] received: 645:Hithere
*/
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

    pipe (we need two for two-way comm)
    + -------------------------------+
    + wR (child)         (parent) r  +
    + -------------------------------+
*/

#define wR  1 //write end of pipe (child sends)
#define r  0  //read end of pipe (parent receives)
#define psize  2 //pipe size

void check(int ret);
void send(char c[], int p[]);
void receive(int p[]);

int main(int argc, char * argv[]) {
    int p[psize], p2[psize];
    check(pipe(p));
    check(pipe(p2));
    int sender = fork();
    check(sender);

    if(sender == 0) { 
        send("Child ",p);
        receive(p2);
        exit(0);
    }

    receive(p);
    send("Parent ",p2);
    wait(NULL);
    return 0;
}

void check(int ret) {
    if(ret<0) {
        printf("Error");
        exit(-1);
    }
}

void send(char c[], int pipe[]) {
    //close unwanted read end of pipe
    check(close(pipe[r]));
    //save stdout for later
    int savedStdOut = dup(STDOUT_FILENO);
    check(savedStdOut);
    //close stdout
    check(close(STDOUT_FILENO));
    //replace it with pipe's write end
    int replace = dup(pipe[wR]);
    check(replace);
    printf("%d:%s",getpid(),c);
    fflush(stdout);
    //close pipe's write end after use
    check(close(replace));
    //restore stdout
    check(dup(savedStdOut));
}

void receive(int pipe[]) {
    //close unwanted write end of pipe
    check(close(pipe[wR]));
    char received[20];
    //save stdin for later
    int savedStdIn = dup(STDIN_FILENO);
    check(savedStdIn);
    //close stdin
    check(close(STDIN_FILENO));
    //replace it with pipe's read end
    int replace = dup(pipe[r]);
    check(replace);
    scanf("%s",received);
    fflush(stdin);
    printf("Process %d received : %s\n", getpid(), received);
    //close pipe's read end after use
    check(close(replace));
    //restor stdin
    check(dup(savedStdIn));
    
}
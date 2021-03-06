#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

/*
    0 = STDIN_FILENO
    1 = STDOUT_FILENO
    2 = STDERR_FILENO
    3 = unsued
    4 = unused
*/

void check(int ret);

int main(int argc, char * argv[]) {
    int p[2];
    pipe(p);
    int sender = fork();
    check(sender);

    if(sender == 0) { //child (sender) writes to pipe
        char hi[] = "Hi there";
        check(close(STDOUT_FILENO));
        check(dup(p[1]));
        write(p[1], hi, strlen(hi)+1); 
        fflush(stdout);
        
        check(close(p[0]));
        check(close(p[1]));
        exit(0);
    }

    //in the parent (receiver) // reads from pipe
    char received[15];
    check(close(STDIN_FILENO));
    check(dup(p[0]));
    read(p[0], received, 15); 
    fflush(stdin);
    printf("2: Parent received: %s\n", received); 

    check(close(p[0]));
    check(close(p[1]));
    wait(NULL);
    return 0;


}

void check(int ret) {
    if(ret<0) {
        printf("Error");
        exit(-1);
    }
}
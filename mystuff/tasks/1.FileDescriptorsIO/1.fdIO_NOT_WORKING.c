#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/*
    0 = STDIN_FILENO
    1 = STDOUT_FILENO
    2 = STDERR_FILENO
    3 = unsued
    4 = unused
*/

void check(int ret);

int main(int argc, char * argv[]) {
    //make sure this file is created before running program!
    check( close(STDIN_FILENO) );
    int inFD = open("./files/stinA.txt", O_WRONLY, 0666);
    check(inFD);
    
    check( close(STDOUT_FILENO) );
    int outFD = open("./files/stout.txt", O_CREAT | O_WRONLY, 0666);
    check(outFD);
    
    //read from inFp, out to stout.
    char input[25];
    check(fread(input, sizeof(input), 20, stdin));
    fflush(stdin);
    printf("%s",input);
    fflush(stdout);

    //check(close(inFD));
   // check(close(outFD));

    return 0;
}


void check(int ret) {
    if(ret<0) {
        printf("Error");
        exit(-1);
    }
}
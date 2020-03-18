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
    FILE * inFp;
    inFp = fopen("./files/stin.txt","w+");
    fputs("Onceuponatime....", inFp);
    
    //must do this!
    rewind(inFp);
    
    check( close(STDOUT_FILENO) );
    int outFD = open("./files/stout.txt", O_CREAT | O_WRONLY, 0666);
    check(outFD);
    
    //read from inFp, out to stout.
    char input[25];
    fscanf(inFp, "%s", input);
    fflush(stdin);
    printf("%s: %s", argv[0], input);
    fflush(stdout);
    
    //cleanup
    check(fclose(inFp));
    check(close(outFD));

    return 0;
}


void check(int ret) {
    if(ret<0) {
        printf("Error");
        exit(-1);
    }
}
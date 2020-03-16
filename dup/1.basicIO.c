#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int STD_OUT = 1;

int main() {

    system("rm -rf out.txt");
    sleep(1); //making sure out.txt is deleted.

    printf("console out 1\n");

    int savedStdOut = dup(STD_OUT);
    if( savedStdOut < 0 ) {
        printf("Error in dup");
        exit(-1);
    }
    
    int outFileDescriptor = open("out.txt", O_CREAT | O_WRONLY, 0666);
    if( outFileDescriptor < 0 ) {
        printf("Error creating/opening file");
        exit(-1);
    }

    int ret = dup2(outFileDescriptor, STD_OUT);
    if( ret < 0 ) {
        printf("Error in dup2");
        exit(-1);
    }

    printf("This should go inside your out.txt file\n");

    //swap back to standardIO
    ret = dup2(savedStdOut, STD_OUT);
    if( ret < 0 ) {
        printf("Error in dup2");
        exit(-1);
    }

    close(outFileDescriptor);

    printf("console out 2\n");

    return 0;
}

//gcc -o ./bin/1.bin 1.basicIO.c -Wall

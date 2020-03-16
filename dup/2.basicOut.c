#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int STD_OUT_FILE_DESCRIPTOR = 1;

int main() {

    system("rm -rf out.txt");
    sleep(1); //making sure out.txt is deleted.

    printf("standard console out 1\n");

    /*
        SavedStdOut is some value returned by OS. we don't know what. Neg is error.
        Save it for swapping back.
    */    
    int savedStdOut = dup(STD_OUT_FILE_DESCRIPTOR); 
    int myOutFileDescriptor = open("out.txt", O_CREAT | O_WRONLY, 0666);
    dup2(myOutFileDescriptor, STD_OUT_FILE_DESCRIPTOR);
    printf("This should go inside your out.txt file\n");

    //swap back to standardIO
    dup2(savedStdOut, STD_OUT_FILE_DESCRIPTOR);
    close(myOutFileDescriptor);
    printf("standard console out 2\n");

    return 0;
}

//gcc -o ./bin/2.bin 2.basicOut.c -Wall

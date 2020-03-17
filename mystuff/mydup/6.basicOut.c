#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int STD_OUT_FILE_DESCRIPTOR = 1;

int main(int argc, char * argv[]) {

    //rm out.txt
    system("rm -rf out.txt");
    
    //making sure out.txt is deleted.
    sleep(1); 

    //Print to standard out (console)
    printf("standard console out A\n");

    //save slot of stdout for later
    int savedStdOut = dup(STD_OUT_FILE_DESCRIPTOR); 

    //print the slot to stdout (console)
    printf("stdout slot is now some random FD: %d\n", savedStdOut);

    //get FD for your outfile..
    int myOutFileDescriptor = open("out.txt", O_CREAT | O_WRONLY, 0666);
    printf("myOutFileDescriptor slot is now some randome FD: %d\n", myOutFileDescriptor);

    //close original stdout slot so it gets freed
    close(STD_OUT_FILE_DESCRIPTOR);
    
    //dup: pushes your fd into slot freed above.
    dup(myOutFileDescriptor);

    //all printf should now go to your fd
    printf("%s: This should go inside your out.txt file\n", argv[0]);

    //swap back to standardIO first by closing your fd..freeing up STD_OUT_FILE_DESCRIPTOR
    close(myOutFileDescriptor);

    //finally swap to original saved above
    dup2(savedStdOut,STD_OUT_FILE_DESCRIPTOR);
    
    //back to standard console out.
    printf("standard console out B\n");

    return 0;
}

//gcc -o ./bin/2.bin 2.basicOut.c -Wall

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int STD_OUT_FILE_DESCRIPTOR = 1;

int main() {

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

    //close original stdout slot so it gets freed
    close(STD_OUT_FILE_DESCRIPTOR);

    //get FD for your outfile..it will automatically get the freed slot above 
    int myOutFileDescriptor = open("out.txt", O_CREAT | O_WRONLY, 0666);

    //all printf should now go to your fd
    printf("This should go inside your out.txt file\n");
    printf("Your FD is now standard OUT [1]: %d\n", myOutFileDescriptor);

    //swap back to standardIO first by closing your fd..freeing up STD_OUT_FILE_DESCRIPTOR
    close(myOutFileDescriptor);

    //finally swap to original saved above
    dup(savedStdOut);
    
    //back to standard console out.
    printf("standard console out B\n");

    return 0;
}

//gcc -o ./bin/2.bin 2.basicOut.c -Wall

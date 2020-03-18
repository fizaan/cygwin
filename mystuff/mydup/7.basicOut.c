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

//This is similar to 4a.c and 4b.c

int main() {

    //rm out.txt
    system("rm -rf out.txt");
    
    //making sure out.txt is deleted.
    sleep(1); 

    //Print to standard out (console)
    printf("standard console out A\n");

    //step 1:save slot of stdout for later // -----> 3
    int savedStdOut = dup(STDOUT_FILENO); 

    //print the slot to stdout (console). Should print 3
    printf("stdout slot is now some random FD: %d\n", savedStdOut);

    //close original stdout slot so it gets freed. Slot 1 is now free
    close(STDOUT_FILENO);

    //get FD for your outfile..it will automatically get the freed slot above.
    // i.e. myOutFileDescriptor = 1
    int myOutFileDescriptor = open("out.txt", O_CREAT | O_WRONLY, 0666);

    //all printf should now go to your fd
    printf("This should go inside your out.txt file\n");
    printf("Your FD is now standard OUT [1]: %d\n", myOutFileDescriptor);

    //swap back to standardIO first by closing your fd..freeing up STDOUT_FILENO
    //1 gets free again
    close(myOutFileDescriptor);

    //finally swap to original saved above
    //3 to 1
    dup(savedStdOut);
    
    //back to standard console out.
    printf("standard console out B\n");

    return 0;
}

//gcc -o ./bin/2.bin 2.basicOut.c -Wall

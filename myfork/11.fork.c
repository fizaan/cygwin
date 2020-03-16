#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int pid[5], status;

    for(int i = 0; i < 5; i++ ) {
        pid[i] = fork();
        if(pid[i] == -1) 
            printf("For failed\n");
        else if(pid[i] == 0)  { // child
            printf("I am the %dth child [%d] of [%d]\n", i, getpid(), getppid());
            
            /*
                make the 5th child sleep for 5 sec..open new cygwin and type
                ps | grep 10.bin
            */
            if(i == 4)
                sleep(5);

            //must exit!
            exit(0);
        }
    }
    
    printf("I am the parent clone [%d]. I will collect the children...\n",getpid());

    for(int i = 0; i < 5; i++ ) {
        int temppid = wait(&status);
            if( temppid == -1)
                printf("Wait failed\n");  
            else if (temppid > 0)
                printf("Child %d terminated success: %d\n", i, temppid);
    } 

    /*
                All children should have exited by now. Make the parent clone
                sleep for 3 seconds.
                ps | grep 10.bin
    */
    sleep(3);
    //do some logic here after children are taken care of.

    printf("Done! This should print only once.");

    return 0;
   
}


/*

EBC+Alifa@ON34C02750570 /cygdrive/c/Users/Alifa/Desktop/cygwin64-workspace/myfork
$ gcc -o ./bin/11.bin 11.fork.c

EBC+Alifa@ON34C02750570 /cygdrive/c/Users/Alifa/Desktop/cygwin64-workspace/myfork
$ ./bin/11.bin
I am the 0th child [1473] of [1472]
I am the 1th child [1474] of [1472]
I am the 2th child [1475] of [1472]
I am the 3th child [1476] of [1472]
I am the parent clone [1472]. I will collect the children...
I am the 4th child [1477] of [1472]
Child 0 terminated success: 1473
Child 1 terminated success: 1474
Child 2 terminated success: 1476
Child 3 terminated success: 1475
Child 4 terminated success: 1477
This should print only once.


*/
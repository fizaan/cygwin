#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    pid_t pid[5];

    for(int i = 0; i < 5; i++ ) {
        pid[i] = fork();
        if(pid[i] == (pid_t) -1) 
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
        pid_t temppid = waitpid(pid[i], NULL, 0);
            if( temppid == (pid_t) -1)
                printf("Wait failed\n");  
            else if (temppid > (pid_t) 0)
                printf("Child %d terminated success: %d\n", i, temppid);
    } 

    /*
                All children should have exited by now. Make the parent clone
                sleep for 3 seconds.
                ps | grep 10.bin
    */
    sleep(3);
    //do some logic here after children are taken care of.

    printf("This should print only once.");
   
}
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

/*
    BAD fork! This program will crash. Always produce
    wait failed
    wait faild...
*/

int main(int argc, char *argv[]) {
    pid_t pid;

    for(int i = 0; i < 5; i++ ) {
        pid = fork();
        if(pid == (pid_t) -1) 
            printf("For failed\n");
        else if(pid == 0)  { // child
            printf("I am a child [%d] of [%d]\n", getpid(), getppid());
            //sleep(1);
            exit(0);
        }
        else   // Parent. Do some logic and Collect children
           do {
               pid = waitpid( (pid_t) -1, NULL, 0);
               if( pid == (pid_t) -1)
                    printf("Wait failed\n");  
               else if (pid > (pid_t) 0)
                   printf("Child terminated success: %d\n", pid);
           } while(pid != (pid_t) 0); //0 means no more children are available
    } 
   
}
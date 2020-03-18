#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

/*
gcc -o 3.bin 3.fork.c -Wall
./3.bin

$ ./3.bin
I am a child [1139] of [1138]
Child terminated success: 1139
I am a child [1140] of [1138]
Child terminated success: 1140
I am a child [1141] of [1138]
Child terminated success: 1141
I am a child [1142] of [1138]
Child terminated success: 1142
I am a child [1143] of [1138]*

*But... 1143 never gets cleaned up!??


*/

int main(int argc, char *argv[]) {
    pid_t pid;

    for(int i = 0; i < 5; i++ ) {
        pid = fork();
        if(pid == (pid_t) -1) 
            printf("For failed\n");
        else if(pid == 0)  { // child
            printf("I am a child [%d] of [%d]\n", getpid(), getppid());
            exit(0);
        }
        else   // Parent. Do some logic and Collect children
           do {
               pid = waitpid( (pid_t) -1, NULL, WNOHANG);
               if( pid == (pid_t) -1)
                    printf("Wait failed\n");  
               else if (pid > (pid_t) 0)
                   printf("Child terminated success: %d\n", pid);
           } while(pid != (pid_t) 0); //0 means no more children are available
    } 
   
}
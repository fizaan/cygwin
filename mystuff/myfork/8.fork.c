#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

/*
gcc -o ./bin/8.bin 8.fork.c -Wall
./bin/8.bin

*/

int main(int argc, char *argv[]) {
    pid_t pid[5];

    for(int i = 0; i < 5; i++ ) {
        pid[i] = fork();
        if(pid[i] == (pid_t) -1) 
            printf("For failed\n");
        else if(pid[i] == 0)  { // child
            printf("I am a child [%d] of [%d]\n", getpid(), getppid());
            sleep(1);
            exit(0);
        }
    }
    
    printf("I am the parent clone [%d]. I will collect the children...\n",getpid());

    for(int i = 0; i < 5; i++ ) {
        pid_t temppid = waitpid(pid[i], NULL, 0);
            if( temppid == (pid_t) -1)
                printf("Wait failed\n");  
            else if (temppid > (pid_t) 0)
                printf("Child terminated success: %d\n", temppid);
    } 
   
}
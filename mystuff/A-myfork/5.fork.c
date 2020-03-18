#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

/*
gcc -o 5.bin 5.fork.c -Wall
./5.bin

$ ./5.bin
I am a child [1255] of [1254]
I am a child [1256] of [1254]
I am a child [1257] of [1254]
I am a child [1258] of [1254]
I am the parent clone [1254]. I will collect the children...
I am a child [1259] of [1254]
Child terminated success: 1255
Child terminated success: 1256
Child terminated success: 1258
Child terminated success: 1257
Child terminated success: 1259


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
    }
    
    printf("I am the parent clone [%d]. I will collect the children...\n",getpid());

    for(int i = 0; i < 5; i++) {
        pid = wait(NULL);
            if( pid == (pid_t) -1)
                printf("Wait failed\n");  
            else if (pid > (pid_t) 0)
                printf("Child terminated success: %d\n", pid);
    }
   
}
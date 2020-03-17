#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

/*
gcc -o 4.bin 4.fork.c -Wall
./4.bin

$ ./4.bin
I am a child [1242] of [1238]
I am a child [1243] of [1238]
I am a child [1244] of [1238]
I am a child [1245] of [1238]
I am the parent clone [1238]. I will collect the children...
I am a child [1246] of [1238]*
Child terminated success: 1242
Child terminated success: 1243
Child terminated success: 1245
Child terminated success: 1244

*Note that 1246 never gets cleaned. Why?
because of this condition while(pid != (pid_t) 0);


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

    do {
        pid = waitpid( (pid_t) -1, NULL, WNOHANG);
            if( pid == (pid_t) -1)
                printf("Wait failed\n");  
            else if (pid > (pid_t) 0)
                printf("Child terminated success: %d\n", pid);
    } while(pid != (pid_t) 0); //0 means no more children are available 
   
}
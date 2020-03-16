#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

/*
gcc -o 6.bin 6.fork.c -Wall
./6.bin

$ ./6.bin
I am a child [1267] of [1266]
I am a child [1268] of [1266]
I am a child [1269] of [1266]
I am a child [1270] of [1266]
I am the parent clone [1266]. I will collect the children...
I am a child [1271] of [1266]*
Child terminated success: 1267
Child terminated success: 1268
Child terminated success: 1270
Child terminated success: 1269

*Note that 1271 never gets cleaned. Why?
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
        pid = waitpid( (pid_t) 0, NULL, WNOHANG);
            if( pid == (pid_t) -1)
                printf("Wait failed\n");  
            else if (pid > (pid_t) 0)
                printf("Child terminated success: %d\n", pid);
    } while(pid != (pid_t) 0); //0 means no more children are available 
   
}